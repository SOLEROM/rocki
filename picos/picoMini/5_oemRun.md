# OEM runs


### ls /oem/usr/bin/

```
RkLunch-stop.sh                   rk_mpi_mb_test                    sample_rgn_stresstest
RkLunch.sh                        rk_mpi_mmz_test                   sample_venc_stresstest
dumpsys                           rk_mpi_rgn_test                   sample_vi
luckfox_adc_test                  rk_mpi_sys_test                   sample_vi_vpss_osd_venc
luckfox_gpio_test                 rk_mpi_tde_test                   simple_adec_ao
luckfox_i2c_test                  rk_mpi_venc_test                  simple_ai
luckfox_pwm_test                  rk_mpi_vi_dup_test                simple_ai_aenc
luckfox_spi_test                  rk_mpi_vi_test                    simple_ao
luckfox_uart_test                 rk_mpi_vpss_test                  simple_dual_camera
modetest                          rk_pwm_test                       simple_fast_client
mpi_enc_test                      rk_rve_sample_test                simple_venc_jpeg
mpp_info_test                     rk_system_test                    simple_venc_osd
rgaImDemo                         rk_time_test                      simple_vi_eptz_test
rk_adc_test                       rk_watchdog_test                  simple_vi_get_frame
rk_event_test                     rkaiq_3A_server                   simple_vi_ivs
rk_gpio_test                      rkipc                             simple_vi_rkaiq
rk_led_test                       rkisp_demo                        simple_vi_rockiva
rk_mpi_adec_test                  sample_ai                         simple_vi_tde
rk_mpi_aenc_test                  sample_ai_aenc                    simple_vi_venc
rk_mpi_af_test                    sample_camera_stresstest          simple_vi_venc_change_resolution
rk_mpi_ai_test                    sample_demo_multi_camera_eptz     simple_vi_venc_combo
rk_mpi_amix_test                  sample_demo_vi_venc               simple_vi_venc_rtsp
rk_mpi_ao_test                    sample_isp_stresstest             simple_vi_venc_wrap
rk_mpi_avio_test                  sample_mulit_isp_stresstest       simple_vi_vpss_venc
rk_mpi_dup_venc_test              sample_multi_vi                   vpu_api_test
```






### /oem/usr/bin/RkLunch.sh

```

#!/bin/sh

echo aaaa >> /tmp/a

rcS()
{
	for i in /oem/usr/etc/init.d/S??* ;do

		# Ignore dangling symlinks (if any).
		[ ! -f "$i" ] && continue

		case "$i" in
			*.sh)
				# Source shell script for speed.
				(
					trap - INT QUIT TSTP
					set start
					. $i
				)
				;;
			*)
				# No sh extension, so fork subprocess.
				$i start
				;;
		esac
	done
}

check_linker()
{
        [ ! -L "$2" ] && ln -sf $1 $2
}

network_init()
{
	ethaddr1=`ifconfig -a | grep "eth.*HWaddr" | awk '{print $5}'`

	if [ -f /data/ethaddr.txt ]; then
		ethaddr2=`cat /data/ethaddr.txt`
		if [ $ethaddr1 == $ethaddr2 ]; then
			echo "eth HWaddr cfg ok"
		else
			ifconfig eth0 down
			ifconfig eth0 hw ether $ethaddr2
		fi
	else
		echo $ethaddr1 > /data/ethaddr.txt
	fi
	ifconfig eth0 up && udhcpc -i eth0
}

post_chk()
{
	#TODO: ensure /userdata mount done
	cnt=0
	while [ $cnt -lt 30 ];
	do
		cnt=$(( cnt + 1 ))
		if mount | grep -w userdata; then
			break
		fi
		sleep .1
	done

	# if ko exist, install ko first
	default_ko_dir=/ko
	if [ -f "/oem/usr/ko/insmod_ko.sh" ];then
		default_ko_dir=/oem/usr/ko
	fi
	if [ -f "$default_ko_dir/insmod_ko.sh" ];then
		cd $default_ko_dir && sh insmod_ko.sh && cd -
	fi

	network_init &
	check_linker /userdata   /usr/www/userdata
	check_linker /media/usb0 /usr/www/usb0
	check_linker /mnt/sdcard /usr/www/sdcard
	# if /data/rkipc not exist, cp /usr/share
	rkipc_ini=/userdata/rkipc.ini
	default_rkipc_ini=/tmp/rkipc-factory-config.ini

	if [ ! -f "/oem/usr/share/rkipc.ini" ]; then
		lsmod | grep sc530ai
		if [ $? -eq 0 ] ;then
			ln -s -f /oem/usr/share/rkipc-500w.ini $default_rkipc_ini
		fi
		lsmod | grep sc4336
		if [ $? -eq 0 ] ;then
			ln -s -f /oem/usr/share/rkipc-400w.ini $default_rkipc_ini
		fi
		lsmod | grep sc3336
		if [ $? -eq 0 ] ;then
			ln -s -f /oem/usr/share/rkipc-300w.ini $default_rkipc_ini
		fi
	fi
	tmp_md5=/tmp/.rkipc-ini.md5sum
	data_md5=/userdata/.rkipc-default.md5sum
	md5sum $default_rkipc_ini > $tmp_md5
	chk_rkipc=`cat $tmp_md5|awk '{print $1}'`
	rm $tmp_md5
	if [ ! -f $data_md5 ];then
		md5sum $default_rkipc_ini > $data_md5
	fi
	grep -w $chk_rkipc $data_md5
	if [ $? -ne 0 ] ;then
		rm -f $rkipc_ini
		echo "$chk_rkipc" > $data_md5
	fi

	if [ ! -f "$default_rkipc_ini" ];then
		echo "Error: not found rkipc.ini !!!"
		exit -1
	fi
	if [ ! -f "$rkipc_ini" ]; then
		cp $default_rkipc_ini $rkipc_ini -f
	fi

	if [ ! -f "/userdata/image.bmp" ]; then
		cp -fa /oem/usr/share/image.bmp /userdata/
	fi

	if [ -d "/oem/usr/share/iqfiles" ];then
		rkipc -a /oem/usr/share/iqfiles &
	else
		rkipc &
	fi
}

rcS

ulimit -c unlimited
echo "/data/core-%p-%e" > /proc/sys/kernel/core_pattern
# echo 0 > /sys/devices/platform/rkcif-mipi-lvds/is_use_dummybuf

echo 1 > /proc/sys/vm/overcommit_memory

post_chk &
```






### /oem/usr/bin/RkLunch-stop.sh

```
#!/bin/sh

rcK()
{
	for i in $(ls /oem/usr/etc/init.d/S??*) ;do

		# Ignore dangling symlinks (if any).
		[ ! -f "$i" ] && continue

		case "$i" in
			*.sh)
				# Source shell script for speed.
				(
					trap - INT QUIT TSTP
					set stop
					. $i
				)
				;;
			*)
				# No sh extension, so fork subprocess.
				$i stop
				;;
		esac
	done
}

echo "Stop Application ..."
killall rkipc
killall udhcpc

cnt=0
while [ 1 ];
do
	sleep 1
	cnt=$(( cnt + 1 ))
	if [ $cnt -eq 8 ]; then
		echo "killall -9 rkipc"
		killall -9 rkipc
		sleep 0.1
		break
	fi
	ps|grep rkipc|grep -v grep
	if [ $? -ne 0 ]; then
		echo "rkipc exit"
		break
	else
		echo "rkipc active"
	fi
done

rcK


```




### /userdata ( data -> userdata )

```
core-731-rkipc            ethaddr.txt               video0
core-779-luckfox_pico_re  image.bmp                 video1
core-796-rkisp_demo       rkipc.ini                 video2
```

## full IPC mode

On boot, RkLunch.sh brings the board up into a full IPC mode (network, camera sensor, ISP, encoder, streaming app, web UI)


```
[root@luckfox root]$ strings /oem/usr/bin/rkipc | grep -iE 'live'
rtmp://127.0.0.1:1935/live/mainstream
rtmp://127.0.0.1:1935/live/substream
/live/0
/live/1
```

```
pass XXXX is from:

cat /userdata/rkipc.ini | grep pass



```

on host :

```
ffplay -rtsp_transport tcp rtsp://admin:XXXXX@172.32.0.93:554/live/0


ffplay -rtsp_transport tcp rtsp://admin:XXXXX@172.32.0.93:554/live/1
```