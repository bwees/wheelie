# Wheelie

A simple modchip to convert proprietary Future Motion BMS packets (RS485) to VESC BMS packets (CAN). 

Connect your existing Future Motion BMS (in charge-only mode) to your VESC to display per-cell voltages, charge current, and temperatures to the controller as if a VESC Smart BMS was connected. A few notes on this project:

 - Not all VESC BMS data nor its complete protocol/features are supported by this project. This project is NOT running [vesc_bms_fw](https://github.com/vedderb/vesc_bms_fw) and instead utilizes the [VESC CAN Protocol](https://github.com/vedderb/bldc/blob/088af43fa6320153162a43ae24b1fa2c2ccc7b4b/datatypes.h#L1179) to update BMS values. This is similar to how `vesc_bms_fw` works but the complete BMS protocol is not implemented.
- This project is based upon the reverse-engineering work from [Owie](https://github.com/lolwheel/Owie). Check out the `lib/bms` directory for a modified version of the BMS communication library.


## Hardware

The goal of this porject was to create a simple and cheap (under $25) method of exposing BMS data inside of VESC. The current hardware setup requires only 2 components and some wire:
- [ESP32-S3 Mini Development Board](https://www.amazon.com/gp/product/B0CJ85F5CG)
- [SN65HVD230 CAN Bus Transceiver](https://www.amazon.com/gp/product/B07ZT7LLSK)
    - Any CAN bus transceiver will work (i.e. TJA1050), the VD230 is just conveniently small and can be soldered directly onto the ESP32-S3 Mini. 
    The ESP32 provides a ground to the board via one of its GPIOs to provide a clean soldering setup. Check out the picture below for soldering configuration
    - CAN Controllers, like the MCP2515 and MCP2551, will NOT work without modification to the code. This project uses the ESP32's build in CAN controller and only requires a transceiver to work
- [IRF540N N-Channel MOSFET](https://www.amazon.com/dp/B07PH6FPZR)
    - This is used to wake the BMS from sleep (we have to connect VBAT to Ground that is on the blue BMS wire)

This project utilizes a [similar trick](https://github.com/lolwheel/Owie?tab=readme-ov-file#receiving-rs485-directly-via-hardware-uart) that Owie does to receive RS485 packets over UART. This saves on cost and complexity during soldering.

A 3D-printed case is in the works to make a clean mounting solution in the electronics housing. 


## Software

Upload the code using PlatformIO to the ESP32-S3 over USB before installing. 

### OTA Uploads
I plan to have an OTA upload system set up soon. My current plan is to utilize a CAN command to enable/disable a WiFi hotspot for OTA uploads. You can then just run a single [lispBM](https://github.com/vedderb/bldc/tree/master/lispBM) command to send the CAN command from the lispBM REPL inside of VESC Tool.

