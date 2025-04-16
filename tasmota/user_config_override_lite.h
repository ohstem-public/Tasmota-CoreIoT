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

// Uncomment below to compile faster with minimum features

#ifdef USE_IMPROV
#undef USE_IMPROV
#endif

#ifdef USE_DOMOTICZ
#undef USE_DOMOTICZ
#endif

#ifdef USE_TASMOTA_DISCOVERY
#undef USE_TASMOTA_DISCOVERY
#endif

#ifdef ROTARY_V1
#undef ROTARY_V1
#endif

#ifdef USE_SONOFF_RF
#undef USE_SONOFF_RF
#endif

#ifdef USE_SONOFF_SC
#undef USE_SONOFF_SC
#endif

#ifdef USE_ARMTRONIX_DIMMERS
#undef USE_ARMTRONIX_DIMMERS
#endif

#ifdef USE_SONOFF_IFAN
#undef USE_SONOFF_IFAN
#endif

#ifdef USE_BUZZER
#undef USE_BUZZER
#endif

#ifdef USE_ARILUX_RF
#undef USE_ARILUX_RF
#endif

#ifdef USE_DEEPSLEEP
#undef USE_DEEPSLEEP
#endif

#ifdef USE_EXS_DIMMER
#undef USE_EXS_DIMMER
#endif

#ifdef USE_BERRY
#undef USE_BERRY
#endif

#ifdef USE_CSE7761
#undef USE_CSE7761
#endif

#ifdef USE_LVGL
#undef USE_LVGL
#endif

#ifdef USE_TIMERS
#undef USE_TIMERS
#endif

#ifdef USE_RULES
#undef USE_CUSE_RULESSE7761
#endif

#ifdef USE_TUYA_MCU
#undef USE_TUYA_MCU
#endif

#ifdef USE_PS_16_DZ
#undef USE_PS_16_DZ
#endif

#ifdef USE_SHUTTER
#undef USE_SHUTTER
#endif

#ifdef USE_SHELLY_DIMMER
#undef USE_SHELLY_DIMMER
#endif

#ifdef USE_LIGHT_VIRTUAL_CT
#undef USEUSE_LIGHT_VIRTUAL_CT_SHUTTER
#endif

#ifdef USE_ENERGY_SENSOR
#undef USE_ENERGY_SENSOR
#endif

#ifdef USE_MCP39F501
#undef USE_MCP39F501
#endif

#ifdef USE_IR_REMOTE
#undef USE_IR_REMOTE
#endif

#ifdef USE_MCP39F501
#undef USE_MCP39F501
#endif

#ifdef USE_DEVICE_GROUPS
#undef USE_DEVICE_GROUPS
#endif

#ifdef USE_PWM_DIMMER
#undef USE_PWM_DIMMER
#endif

#ifdef USE_SONOFF_D1
#undef USE_SONOFF_D1
#endif

#ifdef USE_MCP39F501
#undef USE_MCP39F501
#endif

#ifdef USE_MCP39F501
#undef USE_MCP39F501
#endif

#ifdef USE_WS2812
#undef USE_WS2812
#endif

#ifdef USE_VEML6070
#undef USE_VEML6070
#endif


#endif  // _USER_CONFIG_OVERRIDE_H_
