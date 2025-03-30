/*
  xdrv_128_1_lorae32.ino - LoRa E32 module integration for Tasmota
  Based on Lora E32 UART module communication
*/
#include "my_user_config.h"

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
#include "ArduinoJson.h" 
/****************************************************************************************** */
#define LED_Pin 10
/*********************************************************************************************\
* LoraE32 command
// This variable will be set to true after initialization
bool AddLoginitSuccess1 = false;

/*
  Optional: if you need to pass any command for your device
  Commands are issued in Console or Web Console
  Commands:
    Say_Hello  - Only prints some text. Can be made something more useful...
    Send_Data  - Send data in list data
    Send_Attribute  - Send attribute of Lora Device
    Help       - Prints a list of available commands
*/
const char MyProjectCommands[] PROGMEM = "|" // No Prefix
                                         "Say_Hello|"
                                         "Help|"
                                         "SendLora|"
                                         "SendLoraTelemetry|"
                                         "SendLoraAttribute|"
                                         "SetNameLora|"
                                         "BeginLora|"
                                         "PrintLora";
void (*const MyProjectCommand[])(void) PROGMEM = {
    &CmdSay_Hello, &CmdHelp,&CmdSendLora,
    &CmdSendLoraTelemetry, &CmdSendLoraAttribute,&CmdSetNameLora,&CmdBeginLora,&CmdPrintLora
};
void CmdHelp(void)
{
    AddLog(LOG_LEVEL_INFO, PSTR("Help: Accepted commands "));
    AddLog(LOG_LEVEL_INFO, PSTR("Say_Hello : Print Hello"));
    AddLog(LOG_LEVEL_INFO, PSTR("SendLoraAttribute : Transmit attributes of your lora to other lora"));
    ResponseCmndDone();
}
void CmdSay_Hello(void)
{
    AddLog(LOG_LEVEL_INFO, PSTR("Say_Hello: Hello!"));
    ResponseCmndDone();
}
void CmdSendLora(void){
  if (XdrvMailbox.data_len == 0)
  {
      AddLog(LOG_LEVEL_INFO, PSTR("No data to transmit"));
      ResponseCmndDone();
      return;
  }
  else
  {
    char*data =XdrvMailbox.data;
    AddLog(LOG_LEVEL_INFO, PSTR("Lora Transmit : %s"), data);
    ResponseStatus rs = my_lora_e32.sendMessage(data);
    AddLog(LOG_LEVEL_INFO, rs.getResponseDescription().c_str());
    ResponseCmndDone();
  }
}

void CmdSetNameLora(void){
  if(XdrvMailbox.data_len == 0){
    AddLog(LOG_LEVEL_INFO,PSTR("Can not set name, no data name"));
    ResponseCmndDone();
    return;
  }
  else{
    AddLog(LOG_LEVEL_INFO,PSTR("Set Name Device: "));
    AddLog(LOG_LEVEL_INFO,XdrvMailbox.data);
    device_info.device_name = String(XdrvMailbox.data);
    ResponseCmndDone();
  }
}

void CmdBeginLora(void){
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc,XdrvMailbox.data);
  if(error){
    AddLog(LOG_LEVEL_INFO, "JSON parse error");
  }
  else{

    if(doc.containsKey("channel") && doc.containsKey("addrHigh") && doc.containsKey("addrLow")){
      uint8_t Baudrate = 3;
      uint8_t fixedTransmission = 0;
      uint8_t channel = doc["channel"].as<uint8_t>();
      uint8_t addrHigh = doc["addrHigh"].as<uint8_t>();
      uint8_t addrLow = doc["addrLow"].as<uint8_t>();
      if(doc.containsKey("Baudrate")){
        Baudrate = doc["Baudrate"];
      }
      if(doc.containsKey("fixedTransmission")){
        fixedTransmission = doc["fixedTransmission"];
      }
      configMyLoraE32(channel,addrHigh,addrLow,Baudrate,fixedTransmission);
    }
  }
}
void CmdSendLoraTelemetry(void)
{
    if (XdrvMailbox.data_len == 0)
    {
        AddLog(LOG_LEVEL_INFO, PSTR("No data to transmit"));
        ResponseCmndDone();
        return;
    }
    StaticJsonDocument<58> doc;
    JsonArray arr = doc.createNestedArray(String(device_info.device_name));  // Tạo mảng cho "Device A"
    JsonObject data = arr.createNestedObject();         // Tạo đối tượng trong mảng
    StaticJsonDocument<58> mailboxDoc;
    DeserializationError error = deserializeJson(mailboxDoc, XdrvMailbox.data);
    for (JsonPair p : mailboxDoc.as<JsonObject>())
    {
        data[p.key()] = p.value();  // Gán tất cả các key-value vào data
    }
    String json_string;
    serializeJson(doc,json_string);
    AddLog(LOG_LEVEL_INFO, PSTR("Lora Transmit : %s"), json_string.c_str());
    ResponseStatus rs = my_lora_e32.sendMessage(json_string);
    AddLog(LOG_LEVEL_INFO, rs.getResponseDescription().c_str());
    ResponseCmndDone();
}

void CmdSendLoraAttribute(void){
    StaticJsonDocument<58> doc;
    // Tạo đối tượng "device A" với nested object
    JsonObject deviceA = doc.createNestedObject(device_info.device_name);

    // Thêm dữ liệu vào "device A"
    deviceA["addrHigh"] = device_info.addrHigh;
    deviceA["addrLow"] = device_info.addrLow;
    deviceA["longitude"] = device_info.longitude;
    deviceA["latitude"] = device_info.latitude;
    String json_string;
    serializeJson(doc,json_string);
    AddLog(LOG_LEVEL_INFO, PSTR("Mess from ESP32S3: %s"), json_string.c_str());
    ResponseStatus rs = my_lora_e32.sendMessage(json_string);
    AddLog(LOG_LEVEL_INFO, rs.getResponseDescription().c_str());
    ResponseCmndDone();
}


void CmdSendData(void)
{
    if (XdrvMailbox.data_len == 0)
    {
        AddLog(LOG_LEVEL_INFO, PSTR("SendData: No data provided!"));
        ResponseCmndDone();
        return;
    }

    AddLog(LOG_LEVEL_INFO, PSTR("Receive from console: %s"), XdrvMailbox.data);

    ResponseCmndDone();
}
/*************************************** LOG DETAIL LORA ****************************************************** */
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
void CmdPrintLora(void){
  ResponseStructContainer c = my_lora_e32.getConfiguration();
  Configuration configuration = *(Configuration *)c.data;
  printParameters(configuration);
  ResponseStructContainer cMi;
  cMi = my_lora_e32.getModuleInformation();
  ModuleInformation mi = *(ModuleInformation *)cMi.data;
  printModuleInformation(mi);
}
/************************************************************************************************************ */
// Biến toàn cục
bool initSuccess = false;
String receivedMessage;
/*-----------------------------Biến cục bộ-------------------------*/
char mess[100];

/*-----------------------------------------------------------------*/

void LoraE32Init()
{
  // Gọi hàm khởi tạo từ MY_LORA_E32.h
  initSuccess = true;
  my_lora_e32.begin();
  configMyLoraE32(20, 0x01, 0x02, 3);
  ResponseStructContainer c = my_lora_e32.getConfiguration();
  pinMode(LED_Pin,OUTPUT); // Pin for RPC
  if (c.data == NULL)
  {
    AddLog(LOG_LEVEL_INFO, PSTR("Config NULL"));
    return;
  }
  else
  {
    Configuration configuration = *(Configuration *)c.data;
    // printParameters(configuration);
    ResponseStructContainer cMi;
    cMi = my_lora_e32.getModuleInformation();
    ModuleInformation mi = *(ModuleInformation *)cMi.data;
    // printModuleInformation(mi);
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
      processRpcCommand(rc.data.c_str());
    }
    else
    {
      AddLog(LOG_LEVEL_INFO, PSTR("ERROR!"));
    }
  }
}



void processRpcCommand(const char* jsonMessage){
  StaticJsonDocument<256> doc; // Bộ nhớ cho json
  DeserializationError error = deserializeJson(doc,jsonMessage);
  if(error){
      AddLog(LOG_LEVEL_INFO,PSTR("JSON Parsing Failed"));
      return;
  }
  // Trích xuất dữ liệu từ json
  String device = doc["device"];
  AddLog(LOG_LEVEL_INFO,PSTR("Revice for Deivce:"));
  AddLog(LOG_LEVEL_INFO,device.c_str());
  // uint8_t AddrHigh = doc["AddrHigh"];
  // uint8_t AddrLow = doc["AddrLow"];
  int request_id = doc["data"]["id"];
  const char* method = doc["data"]["method"];
  int params = doc["data"]["params"];
  if(strcmp(device.c_str(),device_info.device_name.c_str()) != 0){
      AddLog(LOG_LEVEL_INFO,PSTR("RPC is not for us, device_name is differnt"));
      return;
  } 
  // else if(AddrHigh != device_info.addrHigh){
  //     AddLog(LOG_LEVEL_INFO,PSTR("RPC is not for us, AddrHigh is differnt"));
  //     return;       
  // }
  // else if(AddrLow != device_info.addrLow){
  //     AddLog(LOG_LEVEL_INFO,PSTR("RPC is not for us, AddrLow is differnt"));
  //     return;             
  // }
  // Thực hiện RPC
  AddLog(LOG_LEVEL_INFO,PSTR("RPC ID: %d"),request_id);
  if(strcmp(method,"POWER1") == 0){
      AddLog(LOG_LEVEL_INFO,PSTR("Set Led to %d"),params);
      digitalWrite(LED_Pin,params);
      my_lora_e32.sendMessage(jsonMessage);
  }
  // else if(){
  /* More RPC in here*/
  // }
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
    case FUNC_COMMAND:
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("Calling My Project Command..."));
      result = DecodeCommand(MyProjectCommands, MyProjectCommand);
      break;
      // case FUNC_EVERY_SECOND:
      //   break;
      //    case FUNC_EVERY_200_MSECOND:
      //    case FUNC_EVERY_100_MSECOND:
    }
  }

  return result;
}
#endif // LORA_E32
