# simple_vi_rockiva



```
cd /oem/usr/ko
sh insmod_ko.sh

rkipc &
sleep 0.2
killall rkipc

simple_vi_rockiva
```


my version in media/samples/simple_test/simple_vi_rockiva.c



```

[root@luckfox ]# simple_vi_rockiva 
Fast Face Detection Demo
Resolution: 1920x1080
Camera ID: 0

rockit log path (null), log_size = 0, can use export rt_log_path=, export rt_log_size= change
log_file = (nil) 
RTVersion        00:00:27-767 {dump              :064} ---------------------------------------------------------
RTVersion        00:00:27-767 {dump              :065} rockit version: git-8cb4d25b8 Tue Feb 28 11:12:39 2023 +0800
RTVersion        00:00:27-767 {dump              :066} rockit building: built- 2023-02-28 15:23:19
RTVersion        00:00:27-767 {dump              :067} ---------------------------------------------------------
(null)           00:00:27-767 {log_level_init    :203} 

 please use echo name=level > /tmp/rt_log_level set log level 
	name: all cmpi mb sys vdec venc rgn vpss vgs tde avs wbc vo vi ai ao aenc adec
	log_level: 0 1 2 3 4 5 6 

rockit default level 4, can use export rt_log_level=x, x=0,1,2,3,4,5,6 change
(null)           00:00:27-767 {read_log_level    :093} text is all=4
(null)           00:00:27-768 {read_log_level    :095} module is all, log_level is 4
RTIsp3x          00:00:27-772 {ispInitDevice     :208} sensor name = m00_b_sc3336 4-0030
RTIsp3x          00:00:27-772 {ispInitDevice     :211} sensor_index = 0
(null)           00:00:27-773 {monitor_log_level :144} #Start monitor_log_level thread, arg:(nil)
RTIsp3x          00:00:27-778 {ispInitDevice     :208} sensor name = m00_b_sc3336 4-0030
RTIsp3x          00:00:27-779 {ispInitDevice     :211} sensor_index = 0
vi_dev_init
RKViDev          00:00:27-785 {vi_set_dev_attr   :440} VI_DEV_ATTR_S all parameter reserved
  === VI ATTRS INFO: ===  
	devId  :      0
	chnId  :      0
	buffcnt:      0
	buffSize:     0
	width:        0
	height:       0
	Maxwidth:     0
	Maxwidth:     0
	streaming:    0
RKViChn          00:00:27-805 {prepareRuntime    :334} ---------------------------------------------------------
RKViChn          00:00:27-805 {prepareRuntime    :336} vi version: 1.86, name:vvi
RKViChn          00:00:27-805 {prepareRuntime    :337} rockit-ko version: vmpi:fb2eed2be49e
RKViChn          00:00:27-805 {prepareRuntime    :338} rockit-ko building: -2023-02-09-11:04:49
RKViChn          00:00:27-805 {prepareRuntime    :339} ---------------------------------------------------------
RKViChn          00:00:27-808 {prepareRuntime    :358} mb pool create success, MBCnt= 2
cmpi             00:00:27-808 {createRuntime     :546} [non-WRAP MODE]: buff size = 3133440
RKViChn          00:00:27-815 {open              :632} use mmap memory the node_buff_type:0 must be extern(1)!
RTIsp3x          00:00:27-818 {ispInitDevice     :208} sensor name = m00_b_sc3336 4-0030
RTIsp3x          00:00:27-819 {ispInitDevice     :211} sensor_index = 0
RTDeviceV4L2     00:00:27-822 {open              :138} open video name(/dev/video11)
RKViChn          00:00:27-823 {setFrameRate      :1153} [vi] dev(0) ch(0) illegal param s32SrcFrameRate(0) s32DstFrameRate(0)
RTDeviceV4L2     00:00:27-823 {ispCameraInfo     :549} current device:/dev/video11 isn't compatible(cap:0x84201000) device,memoryType:1, retry:0
RTDeviceV4L2     00:00:27-823 {ispInitFormat     :726} ioctl VIDIOC_S_FMT OK
RKViChn          00:00:27-825 {start             :813} =========== vi Start startRuntime ===========
RTDeviceV4L2     00:00:27-825 {ispStreamOn       :440} do ispStreamOn start
RTDeviceV4L2     00:00:27-844 {ispStreamOn       :493} do ispStreamOn done
[DEBUG InitIvaCommon:298] InitIvaCommon started

========InitIvaCommon========
[DEBUG InitIvaCommon:301] Setting model path: /userdata/rockiva_data
[DEBUG InitIvaCommon:304] Calling ROCKIVA_Init
[DEBUG InitIvaCommon:311] ROCKIVA_Init successful, handle: 0x5b978
[DEBUG InitIvaCommon:313] Setting frame release callback
[DEBUG InitIvaCommon:315] InitIvaCommon completed successfully
[DEBUG InitIvaFace:321] InitIvaFace started

========InitIvaFace========
[DEBUG InitIvaFace:326] Configuring face task parameters
[DEBUG InitIvaFace:334] Setting callbacks
[DEBUG InitIvaFace:339] Calling ROCKIVA_FACE_Init


```