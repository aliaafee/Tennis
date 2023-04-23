# Tennis

Pong like game that runs on ATtiny85 Microcontroller, with SSD1306 OLED (128x64 0.96 inch) display.

## Preamble

### Setup ArduinoISP

* Build and Upload ArduinoISP Program to an Ardunio UNO / Nano or other appropriate board.
* Add Additional Board Manager URL for ATtiny85 (File > Preferences): https://raw.githubusercontent.com/damellis/attiny/ide-1.6.x-boards-manager/package_damellis_attiny_index.json
* Install Attiny Package from Boards Manager

### Prepare ATtiny85 for Programming

* Hookup Attiny to Arduino ISP (Pin1 -> D10, Pin4 -> GND, Pin5 -> D11, Pin6 -> D12, Pin7 -> D13, Pin8 -> GND)
* Select Board : ATtiny25/45/85
* Select Processor: Attiny85
* Select Clock: Internal 8MHz
* Select port where ArduinoISP is pulgged int.
* Select programmer: Arduino ISP
* Burn Bootloader


## Build and Upload the Project

* Install PlatformIO
* Install required depenencies `pio lib install`
* Update `board` and `upload_port` to appropriate values in `platformio.ini`
* Build `pio run`
* Upload to device `pio run -t upload`