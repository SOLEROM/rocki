# linux stack


## kernel space

* newer RK356x kernels using rknpu v0.8.x. The NPU is exposed as a DRM device 

```
ls -laR /dev/dri/renderD12*
crw-rw----+ 1 root render 226, 128 Aug 25  2024 /dev/dri/renderD128
crw-rw----+ 1 root render 226, 129 Aug 25  2024 /dev/dri/renderD129

```


## user space tools

```
ps aux | grep rknn_server
root         366  4.6  0.0 283932  1408 ?        Ssl  14:15   1:56 /usr/bin/rknn_server

```



## freq

```
grep -H . /sys/class/devfreq/*npu*/{governor,cur_freq,min_freq,max_freq,available_frequencies} 2>/dev/null
/sys/class/devfreq/fde40000.npu/governor:rknpu_ondemand
/sys/class/devfreq/fde40000.npu/cur_freq:0
/sys/class/devfreq/fde40000.npu/min_freq:200000000
/sys/class/devfreq/fde40000.npu/max_freq:900000000
/sys/class/devfreq/fde40000.npu/available_frequencies:200000000 297000000 400000000 600000000 700000000 800000000 900000000
```
