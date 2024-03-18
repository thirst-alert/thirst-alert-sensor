# thirst-alert-sensor

This repo contains the C++ code that runs on our board of choice - the FireBeetle ESP32.

## More info on the board

`coming soon`

## Getting started

Requirements:

- FireBeetle ESP32 drivers - [follow tutorial](https://wiki.dfrobot.com/FireBeetle_ESP32_IOT_Microcontroller(V3.0)__Supports_Wi-Fi_&_Bluetooth__SKU__DFR0478#target_4). Just install the drivers! The Arduino IDE board that DFRobot provides is not well mantained.
- Arduino IDE

First off, add the board to the Arduino IDE. Open `Preferences->Additional Boards Manager URLs` and add `https://espressif.github.io/arduino-esp32/package_esp32_index.json` to the bottom of the list

![Add Board Manager URL](https://storage.googleapis.com/thirst-alert-public-assets/thirst-alert-sensor-repo/1.png)

Navigate to `Tools->Board->Boards Manager...` and install the `esp32` boards pack by `Espressif Systems`.

![Add Board](https://storage.googleapis.com/thirst-alert-public-assets/thirst-alert-sensor-repo/2.png)

You should now be able to select the board at `Tools->Board->ESP32 Arduino->FireBeetle-ESP32`

![Select Board](https://storage.googleapis.com/thirst-alert-public-assets/thirst-alert-sensor-repo/3.png)

The sketch uses the [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino) library, which needs to be installed before use.

![Install NimBLE pt.1](https://storage.googleapis.com/thirst-alert-public-assets/thirst-alert-sensor-repo/4.png)

Just type `nimble` in the search bar and the correct library should pop up

![Install NimBLE pt.2](https://storage.googleapis.com/thirst-alert-public-assets/thirst-alert-sensor-repo/5.png)

Good to go!