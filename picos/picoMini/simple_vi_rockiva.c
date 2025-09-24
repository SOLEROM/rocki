#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>

#include "sample_comm.h"

// #include "system.h"

#ifdef ROCKIVA
#include "rockiva_ba_api.h"
#include "rockiva_det_api.h"
#include "rockiva_face_api.h"
#endif

#define MODEL_DATA_PATH "/userdata/rockiva_data"
#define OUT_CAPTURE_PATH "/tmp"
#define PATH_MAX 260

// Debug macro
#define DEBUG_PRINT(fmt, ...) do { \
    printf("[DEBUG %s:%d] " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    fflush(stdout); \
} while(0)

static RK_S32 g_s32FrameCnt = -1;
RK_S32 s32chnlId = 0;
static bool quit = false;
static bool first_face_detected = false;
static pthread_mutex_t detection_mutex = PTHREAD_MUTEX_INITIALIZER;

// Global variables to store current frame data for immediate saving
static void *g_currentFrameData = NULL;
static RK_U32 g_currentWidth = 0;
static RK_U32 g_currentHeight = 0;
static bool g_frameDataValid = false;
static pthread_mutex_t frame_mutex = PTHREAD_MUTEX_INITIALIZER;

// Optimized image buffer - reduced size for speed
#define MAX_IMAGE_BUFFER_SIZE 2
typedef struct {
    int index;
    int state; // 0:uninitial; 1:unused; 2:using
    RockIvaImage image;
} IvaImageBuf;

typedef struct {
    RockIvaHandle handle;
    RockIvaInitParam commonParams;
    RockIvaFaceTaskParams faceParams;
    IvaImageBuf imageBuf[MAX_IMAGE_BUFFER_SIZE];
    int hasImageBufAlloc;
} IvaAppContext;

static void sigterm_handler(int sig) {
    DEBUG_PRINT("Signal %d received", sig);
    fprintf(stderr, "signal %d\n", sig);
    quit = true;
}

RK_U64 TEST_COMM_GetNowUs() {
    struct timespec time = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (RK_U64)time.tv_sec * 1000000 + (RK_U64)time.tv_nsec / 1000;
}


// Optimized frame processing - store frame data for potential saving
static void *GetMediaBuffer0(void *arg) {
    DEBUG_PRINT("Thread started");
    IvaAppContext *iva_ctx = (IvaAppContext *)arg;
    printf("========%s========\n", __func__);
    int loopCount = 0;
    int s32Ret;
    RK_S32 waitTime = 100;
    int pipeId = 0;

    VIDEO_FRAME_INFO_S stViFrame;
    VI_CHN_STATUS_S stChnStatus;
    int fd;

    DEBUG_PRINT("Allocating image buffer");
    RockIvaImage *image = (RockIvaImage *)malloc(sizeof(RockIvaImage));
    if (!image) {
        DEBUG_PRINT("ERROR: Failed to allocate image buffer");
        return NULL;
    }
    DEBUG_PRINT("Image buffer allocated successfully");

    while (!quit) {
        // Check if first face already detected - exit early if so
        pthread_mutex_lock(&detection_mutex);
        bool should_continue = !first_face_detected;
        pthread_mutex_unlock(&detection_mutex);
        
        if (!should_continue) {
            DEBUG_PRINT("First face already detected, stopping frame processing");
            break;
        }

        DEBUG_PRINT("Processing frame %d", loopCount);
        memset(image, 0, sizeof(RockIvaImage));
        
        DEBUG_PRINT("Calling RK_MPI_VI_GetChnFrame");
        s32Ret = RK_MPI_VI_GetChnFrame(pipeId, s32chnlId, &stViFrame, waitTime);
        
        if (s32Ret == RK_SUCCESS) {
            DEBUG_PRINT("Frame acquired successfully");
            
            DEBUG_PRINT("Getting file descriptor and frame data");
            fd = RK_MPI_MB_Handle2Fd(stViFrame.stVFrame.pMbBlk);
            void *frame_data = RK_MPI_MB_Handle2VirAddr(stViFrame.stVFrame.pMbBlk);

            DEBUG_PRINT("Querying channel status");
            s32Ret = RK_MPI_VI_QueryChnStatus(pipeId, s32chnlId, &stChnStatus);
            if (s32Ret != RK_SUCCESS) {
                DEBUG_PRINT("WARNING: QueryChnStatus failed with %x", s32Ret);
            }
            
            // Store current frame data for potential immediate saving
            pthread_mutex_lock(&frame_mutex);
            g_currentFrameData = frame_data;
            g_currentWidth = stChnStatus.stSize.u32Width;
            g_currentHeight = stChnStatus.stSize.u32Height;
            g_frameDataValid = true;
            pthread_mutex_unlock(&frame_mutex);
            
            DEBUG_PRINT("Preparing image for IVA - width:%d height:%d fd:%d", 
                       stChnStatus.stSize.u32Width, stChnStatus.stSize.u32Height, fd);
            
            // Prepare image for IVA processing
            image->info.width = stChnStatus.stSize.u32Width;
            image->info.height = stChnStatus.stSize.u32Height;
            image->info.format = ROCKIVA_IMAGE_FORMAT_YUV420SP_NV12;
            image->dataAddr = NULL;
            image->dataPhyAddr = NULL;
            image->dataFd = fd;
            image->frameId = stChnStatus.u32CurFrameID;

            DEBUG_PRINT("Pushing frame to IVA - frameId:%d", image->frameId);
            ROCKIVA_PushFrame(iva_ctx->handle, image, NULL);
            DEBUG_PRINT("Frame pushed successfully");
            
            usleep(10000);

            DEBUG_PRINT("Releasing frame");
            s32Ret = RK_MPI_VI_ReleaseChnFrame(pipeId, s32chnlId, &stViFrame);
            if (s32Ret != RK_SUCCESS) {
                DEBUG_PRINT("ERROR: RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
            } else {
                DEBUG_PRINT("Frame released successfully");
            }
            
            // Clear stored frame data after release to prevent access to freed memory
            pthread_mutex_lock(&frame_mutex);
            g_frameDataValid = false;
            g_currentFrameData = NULL;
            pthread_mutex_unlock(&frame_mutex);
            
            loopCount++;
        } else {
            DEBUG_PRINT("Frame timeout or error: %x", s32Ret);
            usleep(1000);
        }

        if ((g_s32FrameCnt >= 0) && (loopCount > g_s32FrameCnt)) {
            DEBUG_PRINT("Frame count limit reached");
            quit = true;
            break;
        }
    }
    
    DEBUG_PRINT("Freeing image buffer");
    free(image);
    DEBUG_PRINT("Thread exiting normally");
    return NULL;
}

void FrameReleaseCallback(const RockIvaReleaseFrames *releaseFrames, void *userdata) {
    DEBUG_PRINT("FrameReleaseCallback called with %d frames", releaseFrames ? releaseFrames->count : -1);
    if (!releaseFrames) {
        DEBUG_PRINT("ERROR: releaseFrames is NULL");
        return;
    }
    
    for (int i = 0; i < releaseFrames->count; i++) {
        DEBUG_PRINT("Releasing frameId = %d", releaseFrames->frames[i].frameId);
    }
    DEBUG_PRINT("FrameReleaseCallback completed");
}

// Simple and fast face detection callback - save original frame immediately
void FaceDetResultCallback(const RockIvaFaceDetResult *result,
                           const RockIvaExecuteStatus status, void *userData) {
    
    DEBUG_PRINT("FaceDetResultCallback called - status:%d", status);
    
    if (!result) {
        DEBUG_PRINT("ERROR: result is NULL");
        return;
    }
    
    if (status != ROCKIVA_SUCCESS) {
        DEBUG_PRINT("Face detection failed with status: %d", status);
        return;
    }
    
    if (result->objNum == 0) {
        DEBUG_PRINT("No faces detected in frame %d", result->frameId);
        return;
    }

    DEBUG_PRINT("Locking detection mutex");
    pthread_mutex_lock(&detection_mutex);
    
    // Check if this is the first face detection
    if (!first_face_detected) {
        DEBUG_PRINT("Processing first face detection");
        first_face_detected = true;
        
        RK_U64 timestamp = TEST_COMM_GetNowUs();
        
        printf("FIRST FACE DETECTED! Frame %d, Face count: %d, Timestamp: %llu us\n", 
               result->frameId, result->objNum, timestamp);
        
        // Log detection details
        DEBUG_PRINT("Logging %d face details", result->objNum);
        for (int i = 0; i < result->objNum; i++) {
            printf("Face %d: Box=(%d,%d,%d,%d) Quality=%d State=%d\n",
                   i,
                   result->faceInfo[i].faceRect.topLeft.x,
                   result->faceInfo[i].faceRect.topLeft.y,
                   result->faceInfo[i].faceRect.bottomRight.x,
                   result->faceInfo[i].faceRect.bottomRight.y,
                   result->faceInfo[i].faceQuality.score,
                   result->faceInfo[i].faceState);
        }
        
        // Save the current frame data immediately (before it gets freed)
        DEBUG_PRINT("Attempting to save current frame");
        pthread_mutex_lock(&frame_mutex);
        if (g_frameDataValid && g_currentFrameData) {
            DEBUG_PRINT("Frame data is valid, proceeding with save");
            
            // Get frame dimensions
            RK_U32 width = g_currentWidth;
            RK_U32 height = g_currentHeight;
            
            // Create filename with timestamp
            char save_path[PATH_MAX];
            snprintf(save_path, PATH_MAX, "%s/first_face_frame_%llu_%dx%d.yuv",
                     OUT_CAPTURE_PATH, timestamp, width, height);
            
            printf("Saving full frame to: %s\n", save_path);
            
            DEBUG_PRINT("Frame data address: %p", g_currentFrameData);
            
            // Calculate frame size (YUV420SP format)
            size_t frame_size = width * height * 3 / 2;
            DEBUG_PRINT("Frame size: %zu bytes (%dx%d)", frame_size, width, height);
            
            // Save frame data immediately
            FILE *fp = fopen(save_path, "wb");
            if (fp) {
                size_t written = fwrite(g_currentFrameData, 1, frame_size, fp);
                fclose(fp);
                
                if (written == frame_size) {
                    printf("SUCCESS: Full frame saved (%zu bytes) to %s\n", written, save_path);
                } else {
                    printf("WARNING: Only wrote %zu of %zu bytes\n", written, frame_size);
                }
            } else {
                DEBUG_PRINT("ERROR: Failed to open file for writing");
                printf("ERROR: Failed to save frame to %s\n", save_path);
                perror("fopen");
            }
        } else {
            DEBUG_PRINT("WARNING: No valid frame data available for saving");
            printf("WARNING: No frame data available for saving\n");
        }
        pthread_mutex_unlock(&frame_mutex);
        
        // Signal to quit immediately after saving
        DEBUG_PRINT("Setting quit flag");
        printf("First face detection and save complete. Exiting...\n");
        quit = true;
    } else {
        DEBUG_PRINT("Face already detected, ignoring subsequent detections");
    }
    
    DEBUG_PRINT("Unlocking detection mutex");
    pthread_mutex_unlock(&detection_mutex);
    DEBUG_PRINT("FaceDetResultCallback completed");
}

int InitIvaCommon(IvaAppContext *ctx) {
    DEBUG_PRINT("InitIvaCommon started");
    printf("\n========%s========\n", __func__);
    
    DEBUG_PRINT("Setting model path: %s", MODEL_DATA_PATH);
    snprintf(ctx->commonParams.modelPath, ROCKIVA_PATH_LENGTH, MODEL_DATA_PATH);

    DEBUG_PRINT("Calling ROCKIVA_Init");
    RockIvaRetCode ret = ROCKIVA_Init(&ctx->handle, ROCKIVA_MODE_VIDEO, &ctx->commonParams, ctx);
    if (ret != ROCKIVA_RET_SUCCESS) {
        DEBUG_PRINT("ERROR: ROCKIVA_Init failed with code %d", ret);
        printf("ROCKIVA_Init error %d\n", ret);
        return -1;
    }
    DEBUG_PRINT("ROCKIVA_Init successful, handle: %p", ctx->handle);

    DEBUG_PRINT("Setting frame release callback");
    ROCKIVA_SetFrameReleaseCallback(ctx->handle, FrameReleaseCallback);
    DEBUG_PRINT("InitIvaCommon completed successfully");
    return 0;
}

// Simplified face detection parameters - no capture needed
static int InitIvaFace(IvaAppContext *ctx) {
    DEBUG_PRINT("InitIvaFace started");
    printf("\n========%s========\n", __func__);

    RockIvaFaceTaskParams *faceParams = &ctx->faceParams;
    
    DEBUG_PRINT("Configuring face task parameters");
    faceParams->faceTaskType.faceCaptureEnable = 0;    // Disabled - we save original frame
    faceParams->faceTaskType.faceAttributeEnable = 0;  // Disabled - not needed
    faceParams->faceTaskType.faceRecognizeEnable = 0;  // Disabled - not needed

    // Disable detection area for full frame processing
    faceParams->faceCaptureRule.detectAreaEn = 0;

    DEBUG_PRINT("Setting callbacks");
    RockIvaFaceCallback callback;
    callback.detCallback = FaceDetResultCallback;
    callback.analyseCallback = NULL;  // No analysis callback needed
    
    DEBUG_PRINT("Calling ROCKIVA_FACE_Init");
    RockIvaRetCode ret = ROCKIVA_FACE_Init(ctx->handle, &ctx->faceParams, callback);
    if (ret != ROCKIVA_RET_SUCCESS) {
        DEBUG_PRINT("ERROR: ROCKIVA_FACE_Init failed with code %d", ret);
        printf("ROCKIVA_FACE_Init error %d\n", ret);
        return -1;
    }

    DEBUG_PRINT("InitIvaFace completed successfully");
    return 0;
}

// Simplified VI initialization
int vi_dev_init() {
    printf("%s\n", __func__);
    int ret = 0;
    int devId = 0;
    int pipeId = devId;

    VI_DEV_ATTR_S stDevAttr;
    VI_DEV_BIND_PIPE_S stBindPipe;
    memset(&stDevAttr, 0, sizeof(stDevAttr));
    memset(&stBindPipe, 0, sizeof(stBindPipe));

    ret = RK_MPI_VI_GetDevAttr(devId, &stDevAttr);
    if (ret == RK_ERR_VI_NOT_CONFIG) {
        ret = RK_MPI_VI_SetDevAttr(devId, &stDevAttr);
        if (ret != RK_SUCCESS) {
            printf("RK_MPI_VI_SetDevAttr %x\n", ret);
            return -1;
        }
    }

    ret = RK_MPI_VI_GetDevIsEnable(devId);
    if (ret != RK_SUCCESS) {
        ret = RK_MPI_VI_EnableDev(devId);
        if (ret != RK_SUCCESS) {
            printf("RK_MPI_VI_EnableDev %x\n", ret);
            return -1;
        }
        
        stBindPipe.u32Num = pipeId;
        stBindPipe.PipeId[0] = pipeId;
        ret = RK_MPI_VI_SetDevBindPipe(devId, &stBindPipe);
        if (ret != RK_SUCCESS) {
            printf("RK_MPI_VI_SetDevBindPipe %x\n", ret);
            return -1;
        }
    }

    return 0;
}

int vi_chn_init(int channelId, int width, int height) {
    int ret;
    int buf_cnt = 2;  // Minimum buffer count for speed
    
    VI_CHN_ATTR_S vi_chn_attr;
    memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
    vi_chn_attr.stIspOpt.u32BufCount = buf_cnt;
    vi_chn_attr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_MMAP;
    vi_chn_attr.stSize.u32Width = width;
    vi_chn_attr.stSize.u32Height = height;
    vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
    vi_chn_attr.enCompressMode = COMPRESS_MODE_NONE;
    vi_chn_attr.u32Depth = 1;  // Reduced depth for speed
    
    ret = RK_MPI_VI_SetChnAttr(0, channelId, &vi_chn_attr);
    ret |= RK_MPI_VI_EnableChn(0, channelId);
    
    if (ret) {
        printf("ERROR: create VI error! ret=%d\n", ret);
        return ret;
    }

    return ret;
}

static RK_CHAR optstr[] = "?::w:h:I:";
static void print_usage(const RK_CHAR *name) {
    printf("usage example:\n");
    printf("\t%s -I 0 -w 1920 -h 1080\n", name);
    printf("\t-w | --width: VI width, Default:1920\n");
    printf("\t-h | --heght: VI height, Default:1080\n");
    printf("\t-I | --camid: camera ctx id, Default 0\n");
}

int main(int argc, char *argv[]) {
    RK_S32 s32Ret = RK_FAILURE;
    RK_U32 u32Width = 1920;
    RK_U32 u32Height = 1080;
    int c;
    int ret = 0;

    while ((c = getopt(argc, argv, optstr)) != -1) {
        switch (c) {
        case 'w':
            u32Width = atoi(optarg);
            break;
        case 'h':
            u32Height = atoi(optarg);
            break;
        case 'I':
            s32chnlId = atoi(optarg);
            break;
        case '?':
        default:
            print_usage(argv[0]);
            return 0;
        }
    }

    printf("Fast Face Detection Demo\n");
    printf("Resolution: %dx%d\n", u32Width, u32Height);
    printf("Camera ID: %d\n\n", s32chnlId);

    signal(SIGINT, sigterm_handler);

	// // ADD INITIALIZATIONS:
    // rk_system_init();        // System-level init
    // rkipc_rockiva_init();    // NPU-specific init
    // rk_isp_init(0, "/oem/usr/share/iqfiles");  // ISP init


    if (RK_MPI_SYS_Init() != RK_SUCCESS) {
        RK_LOGE("rk mpi sys init fail!");
        goto __FAILED;
    }

    vi_dev_init();
    vi_chn_init(s32chnlId, u32Width, u32Height);

    IvaAppContext iva_ctx;
    memset(&iva_ctx, 0, sizeof(IvaAppContext));

    ret = InitIvaCommon(&iva_ctx);
    if (ret != 0) {
        goto __FAILED;
    }

    ret = InitIvaFace(&iva_ctx);
    if (ret != 0) {
        goto __FAILED;
    }

    printf("Starting face detection... Looking for first face\n");
    RK_U64 start_time = TEST_COMM_GetNowUs();

    pthread_t main_thread;
    pthread_create(&main_thread, NULL, GetMediaBuffer0, &iva_ctx);

    // Wait for first detection or manual quit
    while (!quit) {
        usleep(100000);  // 100ms sleep while waiting
        
        // Check if we've been waiting too long (timeout after 30 seconds)
        RK_U64 current_time = TEST_COMM_GetNowUs();
        if (current_time - start_time > 30000000) {  // 30 seconds
            printf("Timeout: No face detected within 30 seconds\n");
            quit = true;
            break;
        }
    }

    RK_U64 end_time = TEST_COMM_GetNowUs();
    printf("Detection completed in %llu microseconds\n", end_time - start_time);

    // Signal quit and wait for thread to finish properly
    DEBUG_PRINT("Signaling thread to quit and waiting for completion");
    quit = true;
    pthread_join(&main_thread, NULL);
    DEBUG_PRINT("Main thread joined successfully");
    
    // Small delay before cleanup to ensure all callbacks are done
    usleep(200000);  // 200ms delay
    DEBUG_PRINT("Starting cleanup");

    // Clean up IVA resources first
    DEBUG_PRINT("Releasing ROCKIVA handle");
    if (iva_ctx.handle) {
        ROCKIVA_Release(iva_ctx.handle);
        iva_ctx.handle = NULL;
    }
    DEBUG_PRINT("ROCKIVA handle released");

    // Clean up VI resources
    DEBUG_PRINT("Disabling VI channel and device");
    s32Ret = RK_MPI_VI_DisableChn(0, s32chnlId);
    DEBUG_PRINT("VI channel disabled with result: %x", s32Ret);
    
    s32Ret = RK_MPI_VI_DisableDev(0);
    DEBUG_PRINT("VI device disabled with result: %x", s32Ret);

__FAILED:
    DEBUG_PRINT("Cleanup completed, exiting with status: %d", s32Ret);
    printf("Face detection demo finished with status: %d\n", s32Ret);
    RK_MPI_SYS_Exit();
    return 0;
}