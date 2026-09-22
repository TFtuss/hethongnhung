Hướng dẫn build & nạp cho project (STM32F103C8T6, BluePill)

Mục đích
- Hướng dẫn cài công cụ thường dùng (stlink-tools, OpenOCD), lệnh debug probe, và các lệnh nạp an toàn.

1) Tổng quan
- Cấu trúc mặc định theo kiểu "Keil-like": mã nguồn .c đặt trong src/, header trong Inc/, artifacts trong build/.
- Makefile mẫu đã có: dùng arm-none-eabi toolchain để biên dịch và OpenOCD để nạp.

2) Cài công cụ
- Debian / Ubuntu:
  sudo apt update
  sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi openocd stlink

- Fedora:
  sudo dnf install arm-none-eabi-gcc binutils-arm-none-eabi openocd stlink

- Arch / Manjaro:
  sudo pacman -S arm-none-eabi-gcc binutils openocd stlink

- macOS (Homebrew):
  brew install arm-none-eabi-gcc open-ocd stlink

- Windows: dùng STM32CubeProgrammer hoặc WSL + các công cụ trên Linux.

Lưu ý: tên gói có thể khác giữa distro; nếu st-info không có, cài gói "stlink" hoặc "stlink-tools" tùy repository.

3) Udev rules (Linux) — cho phép truy cập ST-Link không cần sudo
- Tạo file udev rule, ví dụ /etc/udev/rules.d/49-stlinkv2.rules:

  SUBSYSTEM=="usb", ATTR{idVendor}=="0483", ATTR{idProduct}=="3748", MODE="0666", GROUP="plugdev"

- Reload rules:
  sudo udevadm control --reload-rules && sudo udevadm trigger

4) Các lệnh hữu ích
- Kiểm probe (stlink-tools):
  st-info --probe

- Nạp .bin nhanh bằng st-flash:
  st-flash write build/<TARGET>.bin 0x08000000

- Nạp / debug bằng OpenOCD (an toàn: halt trước khi nạp):
  openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
    -c "init; reset halt; program build/<TARGET>.bin 0x08000000 verify; reset run; exit"

- Nếu cần truy cập GDB server do OpenOCD mở:
  openocd -f interface/stlink.cfg -f target/stm32f1x.cfg
  arm-none-eabi-gdb build/<TARGET>.elf
  (trong gdb) target remote :3333

5) Lỗi thường gặp và cách xử lý nhanh
- "timed out while waiting for target halted" hoặc "init mode failed":
  - Kiểm tra board có nguồn 3.3V.
  - Kiểm tra dây SWD: SWCLK, SWDIO, GND (và NRST nếu dùng reset external).
  - Thử thêm bước reset halt trước khi program (như lệnh OpenOCD ở trên).
  - Thử giảm tốc độ adapter: thêm -c "adapter speed 50".
  - Kiểm tra udev / quyền truy cập (chạy sudo nếu cần).

- Nhiều probe cùng lúc: lấy serial với st-info và chỉ định bằng hla_serial
  - Ví dụ: openocd ... -c "hla_serial <SERIAL>; init; reset halt; program ..."
  - Hoặc: make flash STLINK_SERIAL=<SERIAL>

6) Build + Flash với Makefile hiện tại
- Build (ví dụ):
  make CC=arm-none-eabi-gcc AS=arm-none-eabi-gcc OBJCOPY=arm-none-eabi-objcopy SIZE=arm-none-eabi-size

- Flash (gọi OpenOCD với reset halt):
  make CC=arm-none-eabi-gcc AS=arm-none-eabi-gcc OBJCOPY=arm-none-eabi-objcopy SIZE=arm-none-eabi-size flash

7) Muốn tự động hơn?
- Có thể sửa Makefile để luôn chạy "init; reset halt" trước khi program, hoặc tạo script helper flash.sh. Nói mình nếu muốn mình cập nhật Makefile để làm điều đó mặc định.

