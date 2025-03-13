/*
  xdrv_128_1_lorae32.ino - LoRa E32 module integration for Tasmota
  Based on Lora E32 UART module communication
*/
#include "user_config_override.h"

#ifdef USE_LORA_E32

/*********************************************************************************************\
 * LoRa E32 bridge for Tasmota
 * Communicates with LoRa E32 modules over UART
 * Sends and receives messages via MQTT
\*********************************************************************************************/

#warning **** LoRa E32 Driver is included... ****

#define XDRV_128 128 // Định danh driver trong hệ thống Tasmota
#include "MYLORA_E32.h"
#include <Arduino.h>
// Biến toàn cục
bool initSuccess = false;
String receivedMessage;
/*-----------------------------Biến cục bộ-------------------------*/
char mess[100];

/*-----------------------------------------------------------------*/
void printParameters(struct Configuration configuration)
{
  AddLog(LOG_LEVEL_INFO, PSTR("----------------------------------------"));

  AddLog(LOG_LEVEL_INFO, PSTR("HEAD BIN: %d %d %X"), configuration.HEAD, configuration.HEAD, configuration.HEAD);
  AddLog(LOG_LEVEL_INFO, PSTR("AddH BIN: %d"), configuration.ADDH);
  AddLog(LOG_LEVEL_INFO, PSTR("AddL BIN: %d"), configuration.ADDL);
  AddLog(LOG_LEVEL_INFO, PSTR("Chan BIN: %d -> %s"), configuration.CHAN, configuration.getChannelDescription().c_str());

  AddLog(LOG_LEVEL_INFO, PSTR("SpeedParityBit BIN    : %d -> %s"),
         configuration.SPED.uartParity, configuration.SPED.getUARTParityDescription().c_str());
  AddLog(LOG_LEVEL_INFO, PSTR("SpeedUARTDataRate BIN : %d -> %s"),
         configuration.SPED.uartBaudRate, configuration.SPED.getUARTBaudRate().c_str());
  AddLog(LOG_LEVEL_INFO, PSTR("SpeedAirDataRate BIN  : %d -> %s"),
         configuration.SPED.airDataRate, configuration.SPED.getAirDataRate().c_str());

  AddLog(LOG_LEVEL_INFO, PSTR("OptionTrans BIN       : %d -> %s"),
         configuration.OPTION.fixedTransmission, configuration.OPTION.getFixedTransmissionDescription().c_str());
  AddLog(LOG_LEVEL_INFO, PSTR("OptionPullup BIN      : %d -> %s"),
         configuration.OPTION.ioDriveMode, configuration.OPTION.getIODroveModeDescription().c_str());
  AddLog(LOG_LEVEL_INFO, PSTR("OptionWakeup BIN      : %d -> %s"),
         configuration.OPTION.wirelessWakeupTime, configuration.OPTION.getWirelessWakeUPTimeDescription().c_str());
  AddLog(LOG_LEVEL_INFO, PSTR("OptionFEC BIN         : %d -> %s"),
         configuration.OPTION.fec, configuration.OPTION.getFECDescription().c_str());
  AddLog(LOG_LEVEL_INFO, PSTR("OptionPower BIN       : %d -> %s"),
         configuration.OPTION.transmissionPower, configuration.OPTION.getTransmissionPowerDescription().c_str());

  AddLog(LOG_LEVEL_INFO, PSTR("----------------------------------------"));
}

void printModuleInformation(struct ModuleInformation moduleInformation)
{
  AddLog(LOG_LEVEL_INFO, PSTR("----------------------------------------"));

  AddLog(LOG_LEVEL_INFO, PSTR("HEAD BIN: %d %d %X"), moduleInformation.HEAD, moduleInformation.HEAD, moduleInformation.HEAD);
  AddLog(LOG_LEVEL_INFO, PSTR("Freq.: %X"), moduleInformation.frequency);
  AddLog(LOG_LEVEL_INFO, PSTR("Version  : %X"), moduleInformation.version);
  AddLog(LOG_LEVEL_INFO, PSTR("Features : %X"), moduleInformation.features);

  AddLog(LOG_LEVEL_INFO, PSTR("----------------------------------------"));
}
// struct LoraE32
// {
//   byte High_address;
//   byte LOW_address;
//   float temperature;
//   float humidity;
//   int soil;
// };
// LoraE32 lora_gateway;
// Khởi tạo module LoRa E32
void MyCustomCommand(void)
{
  AddLog(LOG_LEVEL_INFO, PSTR("Custom Command Executed!"));
  // Thực hiện hành động mong muốn ở đây
}

void RegisterMyCommands()
{
  // RegisterCommand("MyCommand", MyCustomCommand, true);
}

void LoraE32Init()
{
  // Gọi hàm khởi tạo từ MY_LORA_E32.h
  // AddLog(LOG_LEVEL_INFO, PSTR("Success"));
  initSuccess = true;
  my_lora_e32.begin();
  ResponseStructContainer c = my_lora_e32.getConfiguration();
  configMyLoraE32(20, 0x01, 0x02, 3);
  if (c.data == NULL)
  {
    AddLog(LOG_LEVEL_INFO, PSTR("Config NULL"));
    return;
  }
  else
  {
    Configuration configuration = *(Configuration *)c.data;
    printParameters(configuration);
    ResponseStructContainer cMi;
    cMi = my_lora_e32.getModuleInformation();
    ModuleInformation mi = *(ModuleInformation *)cMi.data;
    printModuleInformation(mi);
  }
  AddLog(LOG_LEVEL_INFO, PSTR("LoRa E32: Initialized successfully"));
}

// Xử lý gửi/nhận dữ liệu
void LoraE32Processing()
{
  if (!initSuccess)
    return;

  // Kiểm tra có tin nhắn từ LoRa
  if (my_lora_e32.available() > 1)
  {
    ResponseContainer rc = my_lora_e32.receiveMessage();
    if (rc.status.code == 1)
    {
      AddLog(LOG_LEVEL_INFO, PSTR("Receive Mess: "));
      AddLog(LOG_LEVEL_INFO, rc.data.c_str());
    }
    else
    {
      AddLog(LOG_LEVEL_INFO, PSTR("ERROR!"));
    }
  }
}

void soilmoisture()
{
  int soil = analogRead(A0);
  sprintf(mess, "Soil moisture : %d", soil);
  AddLog(LOG_LEVEL_INFO, mess);
  my_lora_e32.sendMessage(mess);
}
// Interface cho Tasmota
bool Xdrv128(uint32_t function)
{

  bool result = false;

  if (FUNC_INIT == function)
  {
    // AddLog(LOG_LEVEL_INFO, PSTR("INIT"));
    LoraE32Init();
    initSuccess = true;
  }
  else if (initSuccess)
  {

    switch (function)
    {
      //    Select suitable interval for polling your function
    // case FUNC_EVERY_SECOND:
    //   AddLog(LOG_LEVEL_INFO, PSTR("EVERY SECOND"));
    //   break;
    case FUNC_EVERY_250_MSECOND:
      LoraE32Processing();
      break;
      // case FUNC_EVERY_SECOND:
      //   soilmoisture();
      //   break;
      //    case FUNC_EVERY_200_MSECOND:
      //    case FUNC_EVERY_100_MSECOND:
    }
  }

  return result;
}
#endif // LORA_E32
