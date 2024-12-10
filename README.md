# ELETRONIC SCALE AND QT APP
Project tạo ra một mô hình cân điện tử để đo khối lượng vật, 
hiển thị kết quả lên màn hình LCD và máy tính, và có một số chức năng điều khiển bằng nút bấm.
## Thành phần cứng:
- STM32F103C8T6
- Cảm biến Loadcell 5kg
- Module HX711
- I2C-LCD
- module HC05
## Phần mềm
- KeilC code stm32
- vscode code app QT
- Qt Desginer
## Sơ đồ khối
<img src="">
# ELETRONIC SCALE với STM32
- Source code:[STM32](https://github.com/Daiiingc/App_For_Scale/tree/main/ELECTRONIC%20SCALE%20STM32) 
- Folder MDK chứa file keilC để build chương trình và nạp code
- Folder USER chứa file code các thành phần cứng và phần trao đổi bản tin giữa STM32 và app thông qua USART
- Folder REFERENCE chứa các thông tin tìm hiểu để làm xây dựng mô hình phần cứng
# QT APP
- Source code: [QT_App](https://github.com/Daiiingc/App_For_Scale/tree/main/App_Qt) 
- Thiết kế giao diện app trên phần mềm Qt Desginer
- Convert file .ui đó thành file .py và lập trình phần logic cho app trên vscode
- Sử dụng thư viện PyQt5 để code và chạy chương trình phần mềm
- Sử dụng thư viện pyserial để trao đổi bản tin dữ liệu giữa app PyQt5 và stm32
- Sau khi hoàn thành thì đóng gói lại vào một file exe để chạy app (File lưu trong folder dist)