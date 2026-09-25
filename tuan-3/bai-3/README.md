# Bài tập 03 - STM32F103 UART DMA

Firmware cho STM32F103C8T6, build trực tiếp trên Linux bằng `arm-none-eabi-gcc`, không dùng KeilC. Chương trình đếm mỗi lần nhấn nút và gửi:

```text
<Lớp><Nhóm>:BTN:<Giá trị nút nhấn>\n\r
```

Ví dụ với cấu hình mặc định `L01` và `G01`:

```text
L01G01:BTN:1\n\r
```

## Phần cứng

- 1 board STM32F103C8T6 Blue Pill.
- 1 nút nhấn thường hở.
- 1 bộ chuyển USB-UART 3.3 V, ví dụ CP2102/CH340.
- 1 ST-Link V2 hoặc CMSIS-DAP để nạp firmware.
- Dây nối và nguồn 3.3 V/USB.

Sơ đồ nối dây:

| Thiết bị | STM32F103C8T6 |
| --- | --- |
| Nút nhấn chân 1 | PA0 |
| Nút nhấn chân 2 | GND |
| USB-UART RX | PA9 / USART1_TX |
| USB-UART TX | PA10 / USART1_RX |
| USB-UART GND | GND |
| ST-Link SWDIO | PA13 |
| ST-Link SWCLK | PA14 |
| ST-Link GND | GND |
| ST-Link 3V3 | 3V3 |

PA0 được cấu hình pull-up nội, nên nút nhấn ở mức thấp khi nhấn. USB-UART phải dùng mức logic 3.3 V; không nối tín hiệu 5 V trực tiếp vào chân MCU.

## Cấu hình lớp/nhóm

Sửa `include/app_config.h` trước khi nộp:

```c
#define CLASS_ID "L01"
#define GROUP_ID "G01"
```

Đổi thành ID lớp và ID nhóm thực tế của bạn.

## Build và nạp trên Linux

```bash
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi stlink-tools
make
make flash
```

Lệnh `make flash` cần ST-Link đang cắm và board được cấp nguồn. Có thể nạp thủ công bằng:

```bash
st-flash --reset write build/bai3.bin 0x08000000
```

## Kiểm tra UART

Tìm cổng serial, thường là `/dev/ttyUSB0` hoặc `/dev/ttyACM0`, rồi mở terminal ở `115200 8N1`:

```bash
picocom -b 115200 /dev/ttyUSB0
```

Mỗi lần nhấn và nhả nút, cửa sổ terminal sẽ nhận một bản tin. Dữ liệu được truyền bằng DMA1 Channel 4, không có vòng lặp chờ cờ UART trong chương trình.

## Video demo và Github

Video nên thể hiện lần lượt: sơ đồ phần cứng, lệnh `make`, lệnh nạp, terminal `115200`, và nhiều lần nhấn nút với giá trị tăng dần. Đẩy toàn bộ thư mục này lên Github rồi thêm link video vào phần mô tả repository hoặc README trước khi nộp.
