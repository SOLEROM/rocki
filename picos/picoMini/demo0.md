# rkipc

/oem/usr/bin/RkLunch.sh

- rkipc -a /oem/usr/share/iqfiles &
+ rkipc -l 3 -a /oem/usr/share/iqfiles 1>>/tmp/log 2>>/tmp/log &


### log

```
rkipc.c][main]:rkipc_ini_path_ is (null), rkipc_iq_file_path_ is /oem/usr/share/iqfiles, rkipc_log_level is 3
[param.c][rk_param_init]:rk_param_init
[param.c][rk_param_init]:g_ini_path_ is /userdata/rkipc.ini
[param.c][rk_param_dump]:section_num is 55
[param.c][rk_param_dump]:section_name is audio.0
[param.c][rk_param_dump]:audio.0:enable = 1
[param.c][rk_param_dump]:audio.0:card_name = hw:0,0
[param.c][rk_param_dump]:audio.0:encode_type = G711A
[param.c][rk_param_dump]:audio.0:format = S16
[param.c][rk_param_dump]:audio.0:sample_rate = 8000
[param.c][rk_param_dump]:audio.0:channels = 2
[param.c][rk_param_dump]:audio.0:frame_size = 1152
[param.c][rk_param_dump]:audio.0:bit_rate = 16000
[param.c][rk_param_dump]:audio.0:input = mic_in
[param.c][rk_param_dump]:audio.0:volume = 50
[param.c][rk_param_dump]:audio.0:enable_aed = 0
[param.c][rk_param_dump]:audio.0:enable_bcd = 0
[param.c][rk_param_dump]:audio.0:enable_vqe = 1
[param.c][rk_param_dump]:audio.0:vqe_cfg = /oem/usr/share/vqefiles/config_aivqe.json
[param.c][rk_param_dump]:audio.0:rt_audio_period_size = 1024
[param.c][rk_param_dump]:section_name is video.source
...
.....



[param.c][rk_param_dump]:event.regional_invasion:rockiva_model_type = small



[param.c][rk_param_dump]:video.0:enable_motion_static_switch = 0
[storage.c][rk_storage_muxer_init_by_id]:begin
[rockiva.c][rkipc_rockiva_init]:begin
[rockiva.c][rkipc_rockiva_init]:ROCKIVA_Init over
[rockiva.c][rkipc_rockiva_init]:(0,0), (9943,0), (9943,11666), (0,11666)
[rockiva.c][rkipc_rockiva_init]:ROCKIVA_BA_Init success
[rockiva.c][rkipc_rockiva_init]:end
[isp.c][rk_isp_init]:g_iq_file_dir_ is /oem/usr/share/iqfiles
[isp.c][rk_isp_init]:cam_id is 0, hdr_mode is close, scenario is normal
[isp.c][sample_common_isp_init]:ID: 0, sensor_name is m00_b_sc3336 4-0030, iqfiles is /oem/usr/share/iqfiles



[param.c][rk_param_dump]:video.0:enable_motion_deblur = 1
[param.c][rk_param_dump]:video.0:enable_motion_static_switch = 0
[storage.c][rk_storage_muxer_init_by_id]:begin

[rockiva.c][rkba_callback]:size is 1
[rockiva.c][rkba_callback]:topLeft:[225,3796], bottomRight:[2934,9413],objId is 8, frameId is 40, score is 55, type is 1
[rockiva.c][rkipc_rknn_object_get]:ba_result->objNum is 1

[rockiva.c][rkba_callback]:size is 1
[rockiva.c][rkba_callback]:topLeft:[208,3796], bottomRight:[2934,9506],objId is 8, frameId is 41, score is 59, type is 1
[rockiva.c][rkipc_rknn_object_get]:ba_result->objNum is 1
[rockiva.c][rkba_callback]:size is 1
[rockiva.c][rkba_callback]:topLeft:[243,3734], bottomRight:[2916,9444],objId is 8, frameId is 42, score is 56, type is 1
[rockiva.c][rkipc_rknn_object_get]:ba_result->objNum is 1
[rockiva.c][rkba_callback]:size is 1
[rockiva.c][rkba_callback]:topLeft:[173,3148], bottomRight:[2847,9753],objId is 8, frameId is 43, score is 49, type is 1
[rockiva.c][rkipc_rknn_object_get]:ba_result->objNum is 1
[rockiva.c][rkba_callback]:size is 1
[rockiva.c][rkba_callback]:topLeft:[190,3333], bottomRight:[2847,9691],objId is 8, frameId is 44, score is 54, type is 1
[rockiva.c][rkipc_rknn_object_get]:ba_result->objNum is 1
[rockiva.c][rkba_callback]:size is 1
[rockiva.c][rkba_callback]:topLeft:[225,3672], bottomRight:[2829,9413],objId is 8, frameId is 45, score is 63, type is 1
[rockiva.c][rkipc_rknn_object_get]:ba_result->objNum is 1
[rockiva.c][rkba_callback]:size is 1
[rockiva.c][rkba_callback]:topLeft:[208,3611], bottomRight:[2760,9567],objId is 8, frameId is 46, score is 61, type is 1
[rockiva.c][rkipc_rknn_object_get]:ba_result->objNum is 1
[rockiva.c][rkba_callback]:size is 1
[rockiva.c][rkba_callback]:topLeft:[225,3796], bottomRight:[2829,9413],objId is 8, frameId is 47, score is 63, type is 1

```