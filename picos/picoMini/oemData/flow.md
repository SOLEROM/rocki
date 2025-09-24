# call flow

```

/oem/usr/bin/RkLunch.sh
    rkipc -a /oem/usr/share/iqfiles &



```




## rkipc

* ref src

rknn232/proj/luckfox-pico/project/app/rkipc/rkipc/src/rv1103_ipc/main.c 

* ref doc : https://github.com/kentfirst/rv1106_ipc/blob/main/project/app/rkipc/rkipc/docs/Rockchip_Developer_Guide_Linux_RKIPC_EN.md


* load the rkipc.ini
```
    rk_param_init(rkipc_ini_path_);  // <-- HERE: Initialize parameter system
```

* that spin the rockiva app:
    ./project/app/rkipc/rkipc/common/rockiva


## /userdata/rkipc.ini

```
rockiva_model_type             = small
```
