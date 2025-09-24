# rebuild imgs


https://wiki.luckfox.com/Luckfox-Pico/Luckfox-Pico-RV1103/Luckfox-Pico-Plus-Mini/Luckfox-Pico-SDK

git clone https://github.com/LuckfoxTECH/luckfox-pico.git


```
./build.sh lunch
[1] RV1103_Luckfox_Pico_Mini
[1] SPI_NAND
[0] Buildroot

./build.sh 
```

## custom 1103 with IPC

```
./build.sh lunch

[12] custom
Which would you like? [0~12][default:0]: 12


13. BoardConfig_IPC/BoardConfig-SPI_NAND-Buildroot-RV1103_Luckfox_Pico_Mini-IPC.mk

./build.sh 
./build.sh firmware
```


## 5.10.110

```

https://codeload.github.com/LuckfoxTECH/luckfox-pico/zip/refs/heads/5.10.110


[7] custom
14. BoardConfig_IPC/BoardConfig-SPI_NAND-Buildroot-RV1103_Luckfox_Pico_Mini_B-IPC.mk


BoardConfig_IPC/BoardConfig-SPI_NAND-Buildroot-RV1103_Luckfox_Pico_Mini_B-IPC.mk

./build.sh 
./build.sh firmware

```


## 

./build.sh clean app
./build.sh clean media
./build.sh media
./build.sh app

./build.sh firmware