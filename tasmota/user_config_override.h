/*
  user_config_override.h - user configuration overrides my_user_config.h for Tasmota

  Copyright (C) 2021  Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _USER_CONFIG_OVERRIDE_H_
#define _USER_CONFIG_OVERRIDE_H_

/*****************************************************************************************************\
 * USAGE:
 *   To modify the stock configuration without changing the my_user_config.h file:
 *   (1) copy this file to "user_config_override.h" (It will be ignored by Git)
 *   (2) define your own settings below
 *
 ******************************************************************************************************
 * ATTENTION:
 *   - Changes to SECTION1 PARAMETER defines will only override flash settings if you change define CFG_HOLDER.
 *   - Expect compiler warnings when no ifdef/undef/endif sequence is used.
 *   - You still need to update my_user_config.h for major define USE_MQTT_TLS.
 *   - All parameters can be persistent changed online using commands via MQTT, WebConsole or Serial.
\*****************************************************************************************************/

/*
Examples :

// -- Master parameter control --------------------
#undef  CFG_HOLDER
#define CFG_HOLDER        4617                   // [Reset 1] Change this value to load SECTION1 configuration parameters to flash

// -- Setup your own Wifi settings  ---------------
#undef  STA_SSID1
#define STA_SSID1         "YourSSID"             // [Ssid1] Wifi SSID

#undef  STA_PASS1
#define STA_PASS1         "YourWifiPassword"     // [Password1] Wifi password

// -- Setup your own MQTT settings  ---------------
#undef  MQTT_HOST
#define MQTT_HOST         "app.coreiot.io" // [MqttHost]

#undef  MQTT_PORT
#define MQTT_PORT         1883                   // [MqttPort] MQTT port (10123 on CloudMQTT)

#undef  MQTT_USER
#define MQTT_USER         "YourMqttUser"         // [MqttUser] Optional user

#undef  MQTT_PASS
#define MQTT_PASS         "YourMqttPass"         // [MqttPassword] Optional password

// You might even pass some parameters from the command line ----------------------------
// Ie:  export PLATFORMIO_BUILD_FLAGS='-DUSE_CONFIG_OVERRIDE -DMY_IP="192.168.1.99" -DMY_GW="192.168.1.1" -DMY_DNS="192.168.1.1"'

#ifdef MY_IP
#undef  WIFI_IP_ADDRESS
#define WIFI_IP_ADDRESS     MY_IP                // Set to 0.0.0.0 for using DHCP or enter a static IP address
#endif

#ifdef MY_GW
#undef  WIFI_GATEWAY
#define WIFI_GATEWAY        MY_GW                // if not using DHCP set Gateway IP address
#endif

#ifdef MY_DNS
#undef  WIFI_DNS
#define WIFI_DNS            MY_DNS               // If not using DHCP set DNS IP address (might be equal to WIFI_GATEWAY)
#endif

#ifdef MY_DNS2
#undef  WIFI_DNS2
#define WIFI_DNS2           MY_DNS2              // If not using DHCP set DNS IP address (might be equal to WIFI_GATEWAY)
#endif

// !!! Remember that your changes GOES AT THE BOTTOM OF THIS FILE right before the last #endif !!!
*/

//#define USE_MQTT_TB_IOT

#ifdef ESP32
#ifdef USER_TEMPLATE
#undef USER_TEMPLATE
#endif
#define USER_TEMPLATE          "{\"NAME\":\"Yolo UNO\",\"GPIO\":[32,1,1,1,1,1,1,1,1,1,1,640,608,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1376,1,1,224],\"FLAG\":0,\"BASE\":1}" // Yolo UNO template
#endif  // End ESP32

#ifdef APP_TIMEZONE
#undef APP_TIMEZONE
#endif
#define APP_TIMEZONE           7

// support energy monitor sensors
#define USE_HLW8012                              // Add support for HLW8012, BL0937 or HJL-01 Energy Monitor for Sonoff Pow and WolfBlitz
#define USE_CSE7766                              // Add support for CSE7766 Energy Monitor for Sonoff S31 and Pow R2
#define USE_PZEM004T                             // Add support for PZEM004T Energy monitor (+2k code)
#define USE_PZEM_AC                              // Add support for PZEM014,016 Energy monitor (+1k1 code)
#define USE_PZEM_DC                              // Add support for PZEM003,017 Energy monitor (+1k1 code)

#ifdef ESP8266
  #ifdef USE_IR_REMOTE
  #undef  USE_IR_REMOTE
  #endif
#endif

#ifdef ESP32
  #define USE_SHT                                // [I2cDriver8] Enable SHT1X sensor (+1k4 code)
    #define USE_HTU                                // [I2cDriver9] Enable HTU21/SI7013/SI7020/SI7021 sensor (I2C address 0x40) (+1k5 code)
  #define USE_BMP                                // [I2cDriver10] Enable BMP085/BMP180/BMP280/BME280 sensors (I2C addresses 0x76 and 0x77) (+4k4 code)
    #define USE_BME68X                           // Enable support for BME680/BME688 sensor using Bosch BME68x library (+6k9 code)
  #define USE_BH1750                             // [I2cDriver11] Enable BH1750 sensor (I2C address 0x23 or 0x5C) (+0k5 code)
    #define USE_VEML6070                           // [I2cDriver12] Enable VEML6070 sensor (I2C addresses 0x38 and 0x39) (+1k5 code)
    #define USE_VEML6070_RSET    270000          // VEML6070, Rset in Ohm used on PCB board, default 270K = 270000ohm, range for this sensor: 220K ... 1Meg
    #define USE_VEML6070_SHOW_RAW                // VEML6070, shows the raw value of UV-A
    #define USE_ADS1115                            // [I2cDriver13] Enable ADS1115 16 bit A/D converter (I2C address 0x48, 0x49, 0x4A or 0x4B) based on Adafruit ADS1x15 library (no library needed) (+0k7 code)
    #define USE_INA219                             // [I2cDriver14] Enable INA219 (I2C address 0x40, 0x41 0x44 or 0x45) Low voltage and current sensor (+1k code)
  #define INA219_SHUNT_RESISTOR (0.100)        // 0.1 Ohm default shunt resistor, can be overriden in user_config_override or using Sensor13
  #define USE_INA226                             // [I2cDriver35] Enable INA226 (I2C address 0x40, 0x41 0x44 or 0x45) Low voltage and current sensor (+2k3 code)
  #define USE_SHT3X                              // [I2cDriver15] Enable SHT3x (I2C address 0x44 or 0x45) or SHTC3 (I2C address 0x70) sensor (+0k7 code)
//  #define USE_TSL2561                            // [I2cDriver16] Enable TSL2561 sensor (I2C address 0x29, 0x39 or 0x49) using library Joba_Tsl2561 (+2k3 code)
//  #define USE_TSL2591                            // [I2cDriver40] Enable TSL2591 sensor (I2C address 0x29) using library Adafruit_TSL2591 (+1k6 code)
//  #define USE_MGS                                // [I2cDriver17] Enable Xadow and Grove Mutichannel Gas sensor using library Multichannel_Gas_Sensor (+10k code)
    #define MGS_SENSOR_ADDR    0x04              // Default Mutichannel Gas sensor i2c address
  #define USE_SGP30                              // [I2cDriver18] Enable SGP30 sensor (I2C address 0x58) (+1k1 code)
  #define USE_SGP40                              // [I2cDriver69] Enable SGP40 sensor (I2C address 0x59) (+1k4 code)
  #define USE_SGP4X                              // [I2cDriver82] Enable SGP41 sensor (I2C address 0x59) (+7k2 code)
//  #define USE_SEN5X                              // [I2cDriver76] Enable SEN5X sensor (I2C address 0x69) (+3k code)
//  #define USE_SI1145                             // [I2cDriver19] Enable SI1145/46/47 sensor (I2C address 0x60) (+1k code)
  #define USE_LM75AD                             // [I2cDriver20] Enable LM75AD sensor (I2C addresses 0x48 - 0x4F) (+0k5 code)
//  #define USE_APDS9960                           // [I2cDriver21] Enable APDS9960 Proximity Sensor (I2C address 0x39). Disables SHT and VEML6070 (+4k7 code)
    #define USE_APDS9960_GESTURE                   // Enable APDS9960 Gesture feature (+2k code)
    #define USE_APDS9960_PROXIMITY                 // Enable APDS9960 Proximity feature (>50 code)
    #define USE_APDS9960_COLOR                     // Enable APDS9960 Color feature (+0.8k code)
    #define USE_APDS9960_STARTMODE  0              // Default to enable Gesture mode
//  #define USE_MCP230xx                           // [I2cDriver22] Enable MCP23008/MCP23017 - Must define I2C Address in #define USE_MCP230xx_ADDR below - range 0x20 - 0x27 (+5k1 code)
//    #define USE_MCP230xx_ADDR 0x20               // Enable MCP23008/MCP23017 I2C Address to use (Must be within range 0x20 through 0x26 - set according to your wired setup)
//    #define USE_MCP230xx_OUTPUT                  // Enable MCP23008/MCP23017 OUTPUT support through sensor29 commands (+2k2 code)
//    #define USE_MCP230xx_DISPLAYOUTPUT           // Enable MCP23008/MCP23017 to display state of OUTPUT pins on Web UI (+0k2 code)
//  #define USE_MCP23XXX_DRV                       // [I2cDriver77] Enable MCP23xxx support as virtual switch/button/relay (+3k(I2C)/+5k(SPI) code)
//  #define USE_PCA9685                            // [I2cDriver1] Enable PCA9685 I2C HW PWM Driver - Must define I2C Address in #define USE_PCA9685_ADDR below - range 0x40 - 0x47 (+1k4 code)
//  #define USE_PCA9685_V2                         // [I2cDriver1] Enable PCA9685 I2C HW PWM Driver - Must define I2C Address in #define USE_PCA9685_ADDR below - range 0x40 - 0x47 (+3k4 code)
//    #define USE_PCA9685_ADDR 0x40                // Enable PCA9685 I2C Address to use (Must be within range 0x40 through 0x47 - set according to your wired setup)
//    #define USE_PCA9685_FREQ 50                  // Define default PWM frequency in Hz to be used (must be within 24 to 1526) - If other value is used, it will rever to 50Hz
//  #define USE_PCA9632                            // [I2cDriver75] Enable PCA9632 I2C HW PWM Driver (+1k8 code)
//    #define USE_PCA9632_ADDR 0x60                // Define PCA9685 I2C Address to use (Must be within range 0x60 through 0x63 - set according to your wired setup)
//    #define USE_PCA9632_CM_0 0                   // Mapping for channel 0
//    #define USE_PCA9632_CM_1 1                   // Mapping for channel 1
//    #define USE_PCA9632_CM_2 2                   // Mapping for channel 2
//    #define USE_PCA9632_CM_3 3                   // Mapping for channel 3
    #define USE_MPR121                             // [I2cDriver23] Enable MPR121 controller (I2C addresses 0x5A, 0x5B, 0x5C and 0x5D) in input mode for touch buttons (+1k3 code)
//  #define USE_CCS811                             // [I2cDriver24] Enable CCS811 sensor (I2C address 0x5A) (+2k2 code)
//  #define USE_CCS811_V2                          // [I2cDriver24] Enable CCS811 sensor (I2C addresses 0x5A and 0x5B) (+2k8 code)
//  #define USE_ENS16x                             // [I2cDriver85] Enable ENS160 and ENS161 sensor (I2C addresses 0x52 and 0x53) (+1.9kB of code and 12B of RAM)
//  #define USE_ENS210                             // [I2cDriver86] Enable ENS210 sensor (I2C addresses 0x43) (+1.7kB of code and 12B of RAM)
    #define USE_MPU6050                            // [I2cDriver25] Enable MPU6050 sensor (I2C address 0x68 AD0 low or 0x69 AD0 high) (+3K3 of code and 188 Bytes of RAM)
      #define USE_MPU6050_DMP                      // Enable in MPU6050 to use the DMP on the chip, should create better results (+8k6 of code)
//  #define USE_MGC3130                            // [I2cDriver27] Enable MGC3130 Electric Field Effect Sensor (I2C address 0x42) (+2k7 code, 0k3 mem)
//  #define USE_MAX44009                           // [I2cDriver28] Enable MAX44009 Ambient Light sensor (I2C addresses 0x4A and 0x4B) (+0k8 code)
  #define USE_SCD30                              // [I2cDriver29] Enable Sensiron SCd30 CO2 sensor (I2C address 0x61) (+3k3 code)
  #define USE_SCD40                              // [I2cDriver62] Enable Sensiron SCd40/Scd41 CO2 sensor (I2C address 0x62) (+3k5 code)
  #define USE_SPS30                              // [I2cDriver30] Enable Sensiron SPS30 particle sensor (I2C address 0x69) (+1.7 code)
//  #define USE_ADE7880                            // [I2cDriver65] Enable ADE7880 Energy monitor as used on Shelly 3EM (I2C address 0x38) (+3k8)
//  #define USE_ADE7953                            // [I2cDriver7] Enable ADE7953 Energy monitor as used on Shelly 2.5 (I2C address 0x38) (+1k5)
  #define USE_VL53L0X                            // [I2cDriver31] Enable VL53L0x time of flight sensor (I2C address 0x29) (+4k code)
//    #define VL53L0X_XSHUT_ADDRESS 0x78           //   VL53L0X base address when used with XSHUT control
  #define USE_VL53L1X                            // [I2cDriver54] Enable VL53L1X time of flight sensor (I2C address 0x29) using Pololu VL53L1X library (+2k9 code)
//    #define VL53L1X_XSHUT_ADDRESS 0x78           //   VL53L1X base address when used with XSHUT control
//    #define VL53L1X_DISTANCE_MODE Long           //   VL53L1X distance mode : Long | Medium | Short
//  #define USE_TOF10120                           // [I2cDriver57] Enable TOF10120 time of flight sensor (I2C address 0x52) (+0k6 code)
    #define USE_MLX90614                           // [I2cDriver32] Enable MLX90614 ir temp sensor (I2C address 0x5a) (+0.6k code)
//  #define USE_CHIRP                              // [I2cDriver33] Enable CHIRP soil moisture sensor (variable I2C address, default 0x20)
//  #define USE_PAJ7620                            // [I2cDriver34] Enable PAJ7620 gesture sensor (I2C address 0x73) (+2.5k code)
//  #define USE_PCF8574                            // [I2cDriver2] Enable PCF8574 I/O Expander (I2C addresses 0x20 - 0x26 and 0x39 - 0x3F) (+2k1 code)
//    #define USE_PCF8574_MODE2                    // Enable Mode2 virtual relays/buttons/switches (+2k3 code)
//    #define USE_PCF8574_SENSOR                   // Enable Mode1 inputs and outputs in SENSOR message (+0k2 code)
//    #define USE_PCF8574_DISPLAYINPUT             // Enable Mode1 inputs display in Web page (+0k2 code)
//    #define USE_PCF8574_MQTTINPUT                // Enable Mode1 MQTT message & rule process on input change detection : stat/%topic%/PCF8574_INP = {"Time":"2021-03-07T16:19:23+01:00","PCF8574-1_INP":{"D1":1}} (+0k5 code)
//    #define PCF8574_ADDR1 0x20                   // First address to search for PCF8574
//    #define PCF8574_ADDR1_COUNT 7                // Number of addresses to search for PCF8574 - Default to 0x20 to 0x26
//    #define PCF8574_ADDR2 0x39                   // First address to search for PCF8574A
//    #define PCF8574_ADDR2_COUNT 6                // Number of addresses to search for PCF8574A - Default to 0x39 to 0x3E
//  #define USE_HIH6                               // [I2cDriver36] Enable Honeywell HIH Humidity and Temperature sensor (I2C address 0x27) (+0k6)
//  #define USE_DHT12                              // [I2cDriver41] Enable DHT12 humidity and temperature sensor (I2C address 0x5C) (+0k7 code)
//  #define USE_DS1624                             // [I2cDriver42] Enable DS1624, DS1621 temperature sensor (I2C addresses 0x48 - 0x4F) (+1k2 code)
//  #define USE_AHT1x                              // [I2cDriver43] Enable AHT10/15 humidity and temperature sensor (I2C address 0x38, 0x39) (+0k8 code)
    #define USE_AHT2x                              // [I2cDriver43] Enable AHT20/AM2301B instead of AHT1x humidity and temperature sensor (I2C address 0x38) (+0k8 code)

    #define USE_RTC_CHIPS                          // Enable RTC chip support and NTP server - Select only one
      #define USE_DS3231                           // [I2cDriver26] Enable DS3231 RTC - used by Ulanzi TC001 (I2C address 0x68) (+1k2 code)
//    #define DS3231_ENABLE_TEMP                   //   In DS3231 driver, enable the internal temperature sensor
//    #define USE_BM8563                           // [I2cDriver59] Enable BM8563 RTC - used by M5Stack - support both I2C buses on ESP32 (I2C address 0x51) (+2.5k code)
      #define USE_PCF85363                         // [I2cDriver66] Enable PCF85363 RTC - used by Shelly 3EM (I2C address 0x51) (+0k7 code)
//    #define USE_RX8010                           // [I2cDriver90] Enable RX8010 RTC - used by IOTTIMER - support both I2C buses on ESP32 (I2C address 0x32) (+0k7 code)
      #define USE_PCF85063                         // [I2cDriver92] Enable PCF85063 RTC support (I2C address 0x51)

    #define USE_DISPLAY                            // Add I2C/TM1637/MAX7219 Display Support (+2k code)
    #define USE_DISPLAY_MODES1TO5                // Enable display mode 1 to 5 in addition to mode 0
    #define USE_DISPLAY_LCD                      // [DisplayModel 1] [I2cDriver3] Enable Lcd display (I2C addresses 0x27 and 0x3F) (+6k code)
#endif

#endif  // _USER_CONFIG_OVERRIDE_H_
