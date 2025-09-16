# after full burn shell

```
[root@luckfox ]$ ls
bin            lib32          oem            run            usr
data           lib64          opt            sbin           var
dev            linuxrc        proc           sys
etc            media          rockchip_test  tmp
lib            mnt            root           userdata
[root@luckfox ]$ ifconfig 
lo        Link encap:Local Loopback  
          inet addr:127.0.0.1  Mask:255.0.0.0
          UP LOOPBACK RUNNING  MTU:65536  Metric:1
          RX packets:652 errors:0 dropped:0 overruns:0 frame:0
          TX packets:652 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:48140 (47.0 KiB)  TX bytes:48140 (47.0 KiB)

usb0      Link encap:Ethernet  HWaddr 06:FD:6B:33:B2:57  
          inet addr:172.32.0.93  Bcast:172.32.255.255  Mask:255.255.0.0
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:82 errors:0 dropped:63 overruns:0 frame:0
          TX packets:22 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:13579 (13.2 KiB)  TX bytes:3694 (3.6 KiB)

[root@luckfox ]$ free -h
              total        used        free      shared  buff/cache   available
Mem:          33.2M       14.8M        1.4M      584.0K       17.1M       15.0M
Swap:             0           0           0
[root@luckfox ]$ df -h
Filesystem                Size      Used Available Use% Mounted on
ubi0:rootfs              67.1M     62.1M      5.0M  93% /
devtmpfs                 16.5M         0     16.5M   0% /dev
tmpfs                    16.6M         0     16.6M   0% /dev/shm
tmpfs                    16.6M     56.0K     16.6M   0% /tmp
tmpfs                    16.6M    528.0K     16.1M   3% /run
/dev/ubi4_0              22.4M     17.1M      5.3M  76% /oem
/dev/ubi5_0               4.5M     24.0K      4.5M   1% /userdata
[root@luckfox ]$ cat /proc/partitions 
major minor  #blocks  name

   1        0       4096 ram0
   1        1       4096 ram1
   1        2       4096 ram2
   1        3       4096 ram3
   1        4       4096 ram4
   1        5       4096 ram5
   1        6       4096 ram6
   1        7       4096 ram7
   1        8       4096 ram8
   1        9       4096 ram9
   1       10       4096 ram10
   1       11       4096 ram11
   1       12       4096 ram12
   1       13       4096 ram13
   1       14       4096 ram14
   1       15       4096 ram15
  31        0        256 mtdblock0
  31        1        256 mtdblock1
  31        2        512 mtdblock2
  31        3       4096 mtdblock3
  31        4      30720 mtdblock4
  31        5      10240 mtdblock5
  31        6      81920 mtdblock6


[root@luckfox ]$ uname  -a
Linux luckfox 5.10.110 #1 Tue Jun 18 16:51:28 CST 2024 armv7l GNU/Linux


[root@luckfox ]$ /bin/busybox  --help
BusyBox v1.36.1 (2024-06-18 17:02:47 CST) multi-call binary.
BusyBox is copyrighted by many authors between 1998-2015.
Licensed under GPLv2. See source distribution for detailed
copyright notices.

Usage: busybox [function [arguments]...]
   or: busybox --list[-full]
   or: busybox --show SCRIPT
   or: busybox --install [-s] [DIR]
   or: function [arguments]...

	BusyBox is a multi-call binary that combines many common Unix
	utilities into a single executable.  Most people will create a
	link to busybox for each function they wish to use and BusyBox
	will act like whatever it was invoked as.

Currently defined functions:
	[, [[, addgroup, adduser, ar, arch, arp, arping, ascii, ash, awk,
	base32, base64, basename, bc, blkid, bunzip2, bzcat, cat, chattr,
	chgrp, chmod, chown, chroot, chrt, chvt, cksum, clear, cmp, cp, cpio,
	crc32, crond, crontab, cut, date, dc, dd, deallocvt, delgroup, deluser,
	devmem, df, diff, dirname, dmesg, dnsd, dnsdomainname, dos2unix, du,
	dumpkmap, echo, egrep, eject, env, ether-wake, expr, factor, fallocate,
	false, fbset, fdflush, fdformat, fdisk, fgrep, find, flock, fold, free,
	freeramdisk, fsck, fsfreeze, fstrim, fuser, getopt, getty, grep,
	gunzip, gzip, halt, hdparm, head, hexdump, hexedit, hostid, hostname,
	hwclock, i2cdetect, i2cdump, i2cget, i2cset, i2ctransfer, id, ifconfig,
	ifdown, ifup, inetd, init, insmod, install, ip, ipaddr, ipcrm, ipcs,
	iplink, ipneigh, iproute, iprule, iptunnel, kill, killall, killall5,
	klogd, last, less, link, linux32, linux64, linuxrc, ln, loadfont,
	loadkmap, logger, login, logname, losetup, ls, lsattr, lsmod, lsof,
	lspci, lsscsi, lsusb, lzcat, lzma, lzopcat, makedevs, md5sum, mdev,
	mesg, microcom, mim, mkdir, mkdosfs, mke2fs, mkfifo, mknod, mkpasswd,
	mkswap, mktemp, modprobe, more, mount, mountpoint, mt, mv, nameif,
	netstat, nice, nl, nohup, nologin, nproc, nslookup, nuke, od, openvt,
	partprobe, passwd, paste, patch, pidof, ping, pipe_progress,
	pivot_root, poweroff, printenv, printf, ps, pwd, rdate, readlink,
	readprofile, realpath, reboot, renice, reset, resize, resume, rm,
	rmdir, rmmod, route, run-init, run-parts, runlevel, sed, seedrng, seq,
	setarch, setconsole, setfattr, setkeycodes, setlogcons, setpriv,
	setserial, setsid, sh, sha1sum, sha256sum, sha3sum, sha512sum, shred,
	sleep, sort, start-stop-daemon, strings, stty, su, sulogin, svc, svok,
	swapoff, swapon, switch_root, sync, sysctl, syslogd, tail, tar, tc,
	tee, telnet, telnetd, test, tftp, time, top, touch, tr, traceroute,
	tree, true, truncate, ts, tsort, tty, ubirename, udhcpc, uevent,
	umount, uname, uniq, unix2dos, unlink, unlzma, unlzop, unxz, unzip,
	uptime, usleep, uudecode, uuencode, vconfig, vi, vlock, w, watch,
	watchdog, wc, wget, which, who, whoami, xargs, xxd, xz, xzcat, yes,
	zcat
[root@luckfox ]$ 


[root@luckfox ]$ ldd /bin/busybox 
	libtirpc.so.3 => /usr/lib/libtirpc.so.3 (0xa6f24000)
	libc.so.0 => /lib/libc.so.0 (0xa6e95000)
	ld-uClibc.so.1 => /lib/ld-uClibc.so.0 (0xa6f55000)


```

