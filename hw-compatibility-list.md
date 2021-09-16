# The Amazing Adapter board v1.x

## ESP32 module compatibility and pressure sensor tested in the nautical barometer project

Almost every ESP32 development board has chosen different pins and which signals to break out. 

I have tried to find commonality for the SPI- and I2C interface signals for some standard ESP32 modules.

Any aspiring ESP module to fit must have all the SPI GPIO on exact all physical pins 
according to the `adapter board schematic` found [here](https://github.com/DebinixTeam/esp32-adapter-board-v1x/blob/master/kicad/esp32-adapter-schematic-v1x.pdf).

## SPI GPIO interface configuration

The list for 2.8" TFT SPI 240x320 shows the used GPIO interface, but TFT_MISO is not connected to the ESP32's but is available at an auxiliary connector (J3).

        TFT_MISO        GPIO 19
        TFT_MOSI        GPIO 23
        TFT_SCLK        GPIO 18
        TFT_CS          GPIO 27
        TFT_DC          GPIO 26
        TFT_RST         GPIO 4

This display configuration is likely the only configuration necessary for various ESP32 development boards. 
With this, no more fuzz with SPI pin assignments - hopefully.

## I2C GPIO interface

The well-established SDA/SCL does not need any additional configuration.

## ESP32 modules

### 20-pins

* `ESP32 TinyPico` module from [Seon Rozenblum](https://unexpectedmaker.com). Buy it [here](https://www.amazon.com/Unexpected-Maker-TinyPICO-USB-C/dp/B0917V5YL3/ref=sr_1_1?dchild=1&m=A2V4DOMZNGA67Y&marketplaceID=ATVPDKIKX0DER&qid=1630760075&refinements=p_4%3AUnexpected%2BMaker&s=merchant-items&sr=1-1&th=1)

### 30-pins

* `ESP32 module` from [Geekcreit](https://www.banggood.com/Geekcreit-ESP32-WiFi+bluetooth-Development-Board-Ultra-Low-Power-Consumption-Dual-Cores-Pins-Unsoldered-p-1214159.html?cur_warehouse=CN&rmmds=buy)

### 36-pins

* `ESP32 DEVKIT V1` module from [Geekcreit-DOIT](https://www.banggood.com/ESP32-Development-Board-WiFi+bluetooth-Ultra-Low-Power-Consumption-Dual-Cores-ESP-32-ESP-32S-Board-p-1109512.html?utm_source=google&utm_medium=cpc_ods&utm_content=ana&utm_campaign=ana-sds-7hot-cnw-cpa-21sum&ad_id=537032142639&gclid=CjwKCAjwyvaJBhBpEiwA8d38vGUwFLiOZbcup5nLfNhI7nY7p0ouCBNeLso4Z4J0s2uPwkU7OCH6rhoCLLgQAvD_BwE&cur_warehouse=CN)

### 38-pins

* `ESP32-DEVKITC-32D` from [Expressif](https://www.mouser.com/ProductDetail/Espressif-Systems/ESP32-DevKitC-32D?qs=%252BEew9%252B0nqrDsObWEpDx6YQ==) 

### 12+16-pins

* `ESP32 Thing Plus` from [Sparkfun](https://www.sparkfun.com/products/15663)


## BME680 based pressure sensors

* BME680 from [Pomoroni](https://shop.pimoroni.com/products/bme680-breakout)
* BME680 from [Sparkfun](https://www.sparkfun.com/products/16466)
* BME680 from [Adafruit](https://learn.adafruit.com/adafruit-bme680-humidity-temperature-barometic-pressure-voc-gas)
* CJMCU-680 from [eBay](https://www.ebay.com/itm/CJMCU-680-BME680-Temperature-Humidity-Pressure-Sensor-Ultra-small-Board-Module-/401528135032?_ul=IN) 


## Tested hardware combinations for the `nautical barometer gold` project on the adapter board v1.x

        Yes: Functional
        Yes*: Tested but needs a minor manual configuration change
        No: Does not work at all, major issues


| ESP32 `20-pins` module | Pomoroni BME680 | Sparkfun BME680 | Adafruit BME680 | Generic CJMCU-680
| -----------------------|-----------------|-----------------|-----------------|-------------------
| TinyPico               | Yes             | Not tested      | Not tested      | Not tested


| ESP32 `30-pins` module | Pomoroni BME680 | Sparkfun BME680 | Adafruit BME680 | Generic CJMCU-680
| -----------------------|-----------------|-----------------|-----------------|-------------------
| Geekcreit              | Yes             | Not tested      | Not tested      | Not tested


| ESP32 `36-pins` module | Pomoroni BME680 | Sparkfun BME680 | Adafruit BME680 | Generic CJMCU-680
| -----------------------|-----------------|-----------------|-----------------|-------------------
| Geekcreit-DOIT         | Yes             | Not tested      | Not tested      | Not tested 


| ESP32 `38-pins` module | Pomoroni BME680 | Sparkfun BME680 | Adafruit BME680 | Generic CJMCU-680
| -----------------------|-----------------|-----------------|-----------------|-------------------
| Expressif              | Not tested      | Not tested      | Not tested      | Not tested


| ESP32 `12+16-pins` module | Pomoroni BME680 | Sparkfun BME680 | Adafruit BME680 | Generic CJMCU-680
| ------------------------|-----------------|-----------------|-------------------|-------------------
| Sparkfun                | Yes*            | Not tested      | Not tested        | Not tested


## Manual configuration changes

`Sparkfun's ESP32 Thing Plus` does not use Arduino's more common SPI assignments. 
Thus, open Arduino's libraries folder and change `MOSI` and `SCK` numbers to correct this.

Edit `User_Setup.h` in the libraries folder `TFT_eSPI`, like so:

        TFT_MOSI        GPIO 18
        TFT_SCLK        GPIO 5






