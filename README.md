# ESP32 E-Paper Display with Web Interface

## Overview
This project uses an **ESP32 microcontroller** to control an **e-paper display** via a **web-based interface**. Users can update text, position, font size, color, and background settings remotely through a web form. The display updates dynamically based on user inputs.

## ⚠️ Disclaimer
This code is **not original** and is a **compilation of various resources** found online. It has been modified and combined to achieve the desired functionality. If you recognize portions of this code from other sources, credit is due to the respective authors.

## Features
- **Wi-Fi Connectivity**: ESP32 connects to a specified SSID.
- **Web-based Control Panel**: Users can modify the text displayed on the e-paper screen.
- **Customizable Display Settings**:
  - Text Content
  - Font Size (9pt, 12pt, 18pt)
  - X/Y Positioning
  - Text & Border Color (Black, Red, White)
  - Background Color
- **Low-Power E-Paper Display**: Text remains visible even after power loss.

## Setup Instructions
### **1. Hardware Requirements**
- ESP32 microcontroller
- E-Paper Display (`GxEPD2_290_C90c`)
- Wi-Fi Network

### **2. Libraries Required**
Make sure you have installed the following libraries in the **Arduino IDE**:
- `WiFi.h` (for Wi-Fi connectivity)
- `WebServer.h` (to host the web interface)
- `GxEPD2_3C.h` (for e-paper display control)
- `Fonts/FreeMonoBold9pt7b.h`, `Fonts/FreeMonoBold12pt7b.h`, `Fonts/FreeMonoBold18pt7b.h` (for font rendering)

### **3. Code Upload**
1. Modify the Wi-Fi credentials in the code:
   ```cpp
   const char* ssid = "YourSSID";
   const char* password = "YourPassword";
   ```
2. Compile and upload the code to the ESP32 using **Arduino IDE**.
3. Open the **Serial Monitor** at `115200 baud` to find the assigned **IP address**.
4. Enter the IP address in a browser to access the web interface.

## Web Interface
The web server allows users to modify the text and display properties. Changes are sent to the ESP32 and immediately reflected on the e-paper display.

## ⚠️ Web Server Status: **Test Mode**
The current implementation is in **test mode** and **should not be used in production** without modifications.
- **No authentication**: Anyone on the network can access and modify the display.
- **No input validation**: Users can input invalid values, potentially causing errors.
- **Limited error handling**: The system does not handle incorrect inputs gracefully.

To improve security and functionality, consider implementing:
- User authentication (e.g., login system)
- Input validation (limit string length, prevent bad values)
- Error handling (prevent crashes and undefined behavior)

## Known Issues & Improvements
- The web interface UI is minimal and could be enhanced with better styling.
- ESP32 memory constraints may limit large text updates.
- Some e-paper displays may require different pin configurations.

## Contributions
If you wish to improve or modify this project, feel free to **fork the repository** and submit a pull request.

## License
This project is released under the **MIT License**. However, since the code is a compilation from multiple sources, please ensure you comply with any applicable licenses from original authors.

---
🚀 **Happy Coding!** 🚀

