# Bai1

Bai tap cau hinh PC13 lam ngo ra va dieu khien LED bang thanh ghi truc tiep.

## Phan cung

- STM32F103C8T6 Blue Pill
- ST-Link V2
- LED onboard tai PC13

## Bien dich tren Linux

```bash
sudo apt update
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi make stlink-tools
make
```

## Nap chuong trinh

```bash
st-flash --reset write firmware.bin 0x08000000
```

## Thay doi chu ky

Sua gia tri `BLINK_PERIOD_MS` trong `main.c`, vi du `500U` hoac `2000U`.

## Video demo

Them link video GitHub/YouTube cua ban tai day.
