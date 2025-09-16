# after burn shell


basic

```

# ls
media          linuxrc        usr            opt            dev
lib64          root           var            oem            bin
lib32          sbin           sys            mnt
rockchip_test  proc           tmp            lib
userdata       data           run            etc


# uname -a
Linux Rockchip 5.10.110 #1 Sat Oct 28 15:21:11 CST 2023 armv7l GNU/Linux

# df -h
Filesystem                Size      Used Available Use% Mounted on
ubi0:rootfs              24.2M      5.4M     18.8M  22% /
devtmpfs                 16.8M         0     16.8M   0% /dev
tmpfs                    16.8M         0     16.8M   0% /dev/shm
tmpfs                    16.8M         0     16.8M   0% /tmp
tmpfs                    16.8M    112.0K     16.7M   1% /run
/dev/ubi5_0              38.5M     17.1M     21.4M  44% /oem
/dev/ubi6_0              24.2M     28.0K     24.2M   0% /userdata


# cat /proc/cpuinfo 
processor	: 0
model name	: ARMv7 Processor rev 5 (v7l)
BogoMIPS	: 17.47
Features	: half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae 
CPU implementer	: 0x41
CPU architecture: 7
CPU variant	: 0x0
CPU part	: 0xc07
CPU revision	: 5

Hardware	: Generic DT based system
Revision	: 0000
Serial		: c0dc4dd6f255a45c


# free -h
             total       used       free     shared    buffers     cached
Mem:         34472      15432      19040        112          0       2864
-/+ buffers/cache:      12568      21904
Swap:            0          0          0
# 
# 


```

busybox

```
# bin/busybox  --help
BusyBox v1.27.2 (2023-10-28 15:22:37 CST) multi-call binary.
BusyBox is copyrighted by many authors between 1998-2015.
Licensed under GPLv2. See source distribution for detailed
copyright notices.

Usage: busybox [function [arguments]...]
   or: busybox --list[-full]
   or: busybox --install [-s] [DIR]
   or: function [arguments]...

	BusyBox is a multi-call binary that combines many common Unix
	utilities into a single executable.  Most people will create a
	link to busybox for each function they wish to use and BusyBox
	will act like whatever it was invoked as.

Currently defined functions:
	[, [[, arp, arping, ash, awk, basename, blkid, cat, chattr, chgrp,
	chmod, chown, chroot, chrt, chvt, cksum, clear, cp, cut, date, dd,
	deallocvt, devmem, df, dirname, dmesg, dnsd, dnsdomainname, du, echo,
	env, ether-wake, expr, factor, fallocate, false, find, flock, free,
	fsck, fsfreeze, fstrim, fuser, getty, grep, halt, head, hexdump,
	hostname, hwclock, i2cdetect, i2cdump, i2cget, i2cset, id, ifconfig,
	ifdown, ifup, inetd, init, insmod, ipcrm, ipcs, kill, killall,
	killall5, less, linuxrc, ln, login, logname, ls, lsattr, lsmod, lsof,
	md5sum, mdev, mkdir, mkfifo, mknod, mktemp, mount, mountpoint, mt, mv,
	nc, netstat, nice, nohup, nproc, passwd, ping, pipe_progress,
	pivot_root, poweroff, printenv, ps, pwd, rdate, readlink, realpath,
	reboot, renice, rm, rmmod, route, run-parts, runlevel, rx, sed, seq,
	setkeycodes, setlogcons, setpriv, setserial, setsid, sh, shred, sleep,
	sort, ssl_client, start-stop-daemon, strings, stty, svc, swapoff,
	swapon, switch_root, sync, sysctl, tail, tar, taskset, tee, telnetd,
	test, tftp, timeout, top, touch, tr, true, truncate, tty, udhcpc,
	uevent, umount, uname, uniq, unlink, uptime, usleep, uudecode,
	uuencode, vconfig, vi, w, watch, watchdog, wc, which, who, whoami,
	xargs, yes
# 
```

npu

```
# dmesg  | grep -i npu
[    0.119627] input: adc-keys as /devices/platform/adc-keys/input/input0
[    1.714776] udevd[64]: specified group 'input' unknown
[    8.424536] rkisp_hw ffa00000.rkisp: max input:0x0@0fps
[    8.771305] RKNPU ff660000.npu: RKNPU: rknpu iommu device-tree entry not found!, using non-iommu mode
[    8.772252] RKNPU ff660000.npu: RKNPU: Initialized RKNPU driver: v0.8.2 for 20220829
[    8.772373] RKNPU ff660000.npu: dev_pm_opp_set_regulators: no regulator (rknpu) found: -19
# 

```

net

```
# ifconfig 
lo        Link encap:Local Loopback  
          inet addr:127.0.0.1  Mask:255.0.0.0
          UP LOOPBACK RUNNING  MTU:65536  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

usb0      Link encap:Ethernet  HWaddr D2:DA:1E:FC:4E:29  
          inet addr:172.32.0.93  Bcast:172.32.255.255  Mask:255.255.0.0
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:86 errors:0 dropped:66 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:14134 (13.8 KiB)  TX bytes:0 (0.0 B)


/etc/init.d/S99usb0config 
===========================
#!/bin/sh
TARGET_IP="172.32.0.93"


```