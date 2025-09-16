# linux burn

boot button to connect to the host

Luckfox Pico Plus/Mini B is in flashing mode

```
> lsusb | grep Rocok

Bus 003 Device 012: ID 2207:110c Fuzhou Rockchip Electronics Company 

```

## burn small basic 

```
 35M Oct 28  2023 firmware/backup/luckfox_pico_mini_b_image/update.img
```


```
~/proj/luckFox/flashTools/upgrade_tool_v2.17_for_linux$  sudo ./upgrade_tool uf   ../../firmware/backup/luckfox_pico_mini_b_image/update.img 
[sudo] password for user: 
Using /home/user/proj/luckFox/flashTools/upgrade_tool_v2.17_for_linux/config.ini
Loading firmware...
Support Type:1106	FW Ver:0.0.00	FW Time:2023-10-28 15:25:33
Loader ver:1.01	Loader Time:2023-10-28 15:21:04
Start to upgrade firmware...
Download Boot Start
Download Boot Success
Wait For Maskrom Start
Wait For Maskrom Success
Test Device Start
Test Device Success
Check Chip Start
Check Chip Success
Get FlashInfo Start
Get FlashInfo Success
Prepare IDB Start
Prepare IDB Success
Download IDB Start
Download IDB Success
Download Firmware Start
Download Image... (100%)
Download Firmware Success
Upgrade firmware ok.

```

connect:  adb shell


## burn prebuild


```
96M	/home/user/proj/luckFox/firmware/buildroot/backup/2024.06.05/Luckfox_Pico_Mini_B_Flash/update.img

```

```

~/proj/luckFox/flashTools/upgrade_tool_v2.17_for_linux$ sudo ./upgrade_tool uf ~/proj/luckFox/firmware/buildroot/backup/2024.06.05/Luckfox_Pico_Mini_B_Flash/update.img 

```

connect:  adb shell
