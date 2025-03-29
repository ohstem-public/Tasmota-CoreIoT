/*
  xdrv_103_lorae32_gateway.ino - LoRa E32 module integration for Tasmota
  Based on Lora E32 UART module communication extend for tb gateway
*/
#ifdef USE_LORA_E32_G

/*********************************************************************************************\
 * LoRa E32 bridge for Tasmota
 * Communicates with LoRa E32 modules over UART
 * Sends and receives messages via MQTT
\*********************************************************************************************/

#warning **** LoRa E32 Gateway Driver is included... ****

#define XDRV_103 103 // Định danh driver trong hệ thống Tasmota
#include "MYLORA_E32.h"
#include <Arduino.h>
#define TYPE_TELEMETRY  0
#define TYPE_ATTRIBUTE  1
#define TYPE_RPC        2
#define TYPE_UNKNOWN   -1

bool initSuccess = false;
/*-----------------------------Biến cục bộ-------------------------*/
const char LoRaE32GatewayCommands[] PROGMEM = "|" // No Prefix
                                             "SendLora|"
                                             "e32test|"
                                             "e32testset";
void (*const LoRaE32GatewayCommand[])(void) PROGMEM = {
    &CmdSendLora,
    &e32testCommand, &e32testsetCommand};
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
void CmdSendLora(void)
{
    if (XdrvMailbox.data_len == 0)
    {
        AddLog(LOG_LEVEL_INFO, PSTR("Nothing to transmit"));
        ResponseCmndDone();
        return;
    }
    char *tran = XdrvMailbox.data;
    AddLog(LOG_LEVEL_INFO, PSTR("Transmit data: %s"), tran);
    ResponseStatus rs = my_lora_e32.sendMessage(tran);
    AddLog(LOG_LEVEL_INFO, rs.getResponseDescription().c_str());
    ResponseCmndDone();
}
void e32testCommand(void)
{
  AddLog(LOG_LEVEL_INFO, PSTR("Custom Command Executed!"));
  ResponseCmndDone();
}
void e32testsetCommand(void)
{
  AddLog(LOG_LEVEL_INFO, PSTR("Custom1 Command Executed!"));
  ResponseCmndDone();
}

void LoraE32Init()
{
  // Gọi hàm khởi tạo từ MY_LORA_E32.h
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
int parseJsonType(String jsonStr) {
  // Convert to char
  int len = jsonStr.length() + 1;
  char json[len];
  jsonStr.toCharArray(json, len);

  JsonParser parser(json);
  if (!parser) {
      return TYPE_UNKNOWN;
  }
  JsonParserObject root = parser.getRootObject();
  // contain key "device" and "id" -> RPC Response
  if (root["device"] && root["id"]) {
      return TYPE_RPC;
  }
  // key is Object -> Attribute
  for (auto key : root) {
      if (key.getValue().isObject()) {
          return TYPE_ATTRIBUTE;
      }
  }
  // key is Array -> Telemetry
  for (auto key : root) {
      if (key.getValue().isArray()) {
          return TYPE_TELEMETRY;
      }
  }
  return TYPE_UNKNOWN;
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

      int msg_type = parseJsonType(rc.data);
      String topic_target("gateway/DEVICE/");
      if(msg_type == TYPE_TELEMETRY){
        // AddLog(LOG_LEVEL_INFO, PSTR("TYPE_TELEMETRY"));
        topic_target += "telemetry";
      }else if(msg_type == TYPE_ATTRIBUTE){
        // AddLog(LOG_LEVEL_INFO, PSTR("TYPE_ATTRIBUTE"));
        topic_target += "attribute";
      }else if(msg_type == TYPE_RPC){
        // AddLog(LOG_LEVEL_INFO, PSTR("TYPE_RPC"));
        topic_target += "rpc";
      }else return;
      MqttPublishPayload(topic_target.c_str(),rc.data.c_str());
    }
    else
    {
      AddLog(LOG_LEVEL_INFO, PSTR("ERROR!"));
    }
  }
}

// Interface cho Tasmota
bool Xdrv103(uint32_t function)
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
    //   break;
    case FUNC_EVERY_250_MSECOND:
      LoraE32Processing();
      break;
      //    case FUNC_EVERY_200_MSECOND:
      //    case FUNC_EVERY_100_MSECOND:
      case FUNC_COMMAND:
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("Calling Lora_E32 CMD..."));
      result = DecodeCommand(LoRaE32GatewayCommands, LoRaE32GatewayCommand);
      break;
    }
  }

  return result;
}
#endif // LORA_E32
