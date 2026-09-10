# Bai 2 - STM32F103 bare-metal

Dieu khien 8 LED noi vao PA0-PA7. LED chay tu PA0 den PA7, sau do dao chieu
tu PA7 ve PA0 va lap lai.

Chuong trinh chi dung thanh ghi RCC va GPIOA, khong dung STD, HAL hay SPL.
Mac dinh LED active-high.

## Build

```bash
cd /home/phuc/baitaphtn/bai2
make clean
make
```

## Nap qua ST-Link

```bash
make info
make flash
```
Lenh truc tiep:

```bash
st-flash --reset write build/bai2.bin 0x08000000
```

Ket noi SWDIO, SWCLK, GND va 3.3V. 
