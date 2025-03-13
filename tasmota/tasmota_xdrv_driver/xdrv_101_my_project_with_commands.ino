
/*
  xdrv_101_my_project_with_commands.ino - My IoT device support for Tasmota
*/
#define USE_MY_PROJECT_CMD
#ifdef USE_MY_PROJECT_CMD
#include "MYLORA_E32.h"
/*********************************************************************************************\
 * My IoT Device with command support
 *
 *
\*********************************************************************************************/

#warning **** My IoT Device Driver with commands is included... ****

#define XDRV_101 101

/*********************************************************************************************\
 * My IoT Device Functions
\*********************************************************************************************/

// This variable will be set to true after initialization
bool AddLoginitSuccess1 = false;

/*
  Optional: if you need to pass any command for your device
  Commands are issued in Console or Web Console
  Commands:
    Say_Hello  - Only prints some text. Can be made something more useful...
    SendMQTT   - Send a MQTT example message
    Help       - Prints a list of available commands
*/
char trans_data[1000];
const char MyProjectCommands[] PROGMEM = "|" // No Prefix
                                         "Say_Hello|"
                                         "SendMQTT|"
                                         "SendData|"
                                         "Help|"
                                         "SendLoRa|"
                                         "AddData";
void (*const MyProjectCommand[])(void) PROGMEM = {
    &CmdSay_Hello, &CmdSendMQTT, &CmdSendData, &CmdHelp,
    &CmdSendLora, &CmdAddData};
void CmdSay_Hello(void)
{
    AddLog(LOG_LEVEL_INFO, PSTR("Say_Hello: Hello!"));
    ResponseCmndDone();
}

void CmdAddData(void)
{
    if (XdrvMailbox.data_len == 0)
    {
        AddLog(LOG_LEVEL_INFO, PSTR("AddData: No data provided!"));
        ResponseCmndDone();
        return;
    }

    // Sao chép dữ liệu JSON vào trans_data
    strncpy(trans_data, XdrvMailbox.data, sizeof(trans_data) - 1);
    trans_data[sizeof(trans_data) - 1] = '\0'; // Đảm bảo chuỗi kết thúc an toàn
    // In ra nội dung JSON để debug
    AddLog(LOG_LEVEL_INFO, PSTR("AddData received: %s"), trans_data);
    ResponseCmndDone();
}

void CmdSendLora(void)
{
    if (XdrvMailbox.data_len == 0)
    {
        AddLog(LOG_LEVEL_INFO, PSTR("SendData: Send deivce!"));
        ResponseStatus rs = my_lora_e32.sendMessage(trans_data);
        ResponseCmndDone();
        return;
    }
    char *tran = XdrvMailbox.data;
    AddLog(LOG_LEVEL_INFO, PSTR("Mess from ESP32S3: %s"), tran);
    ResponseStatus rs = my_lora_e32.sendMessage(tran);
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
char payload[200];
char topic[100];

void CmdSendMQTT(void)
{
    AddLog(LOG_LEVEL_INFO, PSTR("Sending MQTT message."));
    snprintf_P(topic, sizeof(topic), PSTR("tasmota/myproject"));

    snprintf_P(payload, sizeof(payload),
               PSTR("{\"" D_JSON_TIME "\":\"%s\",\"name\":\"My Project\"}"),
               GetDateAndTime(DT_LOCAL).c_str());

    // retain = true
    MqttPublishPayload(topic, payload, strlen(payload), false);

    ResponseCmndDone();
}

void CmdHelp(void)
{
    AddLog(LOG_LEVEL_INFO, PSTR("Help: Accepted commands - Say_Hello, SendMQTT, Help"));
    ResponseCmndDone();
}

/*********************************************************************************************\
 * Tasmota Functions
\*********************************************************************************************/

void MyProjectInit()
{

    /*
      Here goes My Project setting.
      Usually this part is included into setup() function
    */

    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("My Project init..."));

    Serial.begin(115200);

    // Set AddLoginitSuccess1 at the very end of the init process
    // Init is successful
    AddLoginitSuccess1 = true;
}

void MyProjectProcessing(void)
{

    /*
      Here goes My Project code.
      Usually this part is included into loop() function
    */
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xdrv101(uint32_t function)
{

    bool result = false;

    if (FUNC_INIT == function)
    {
        MyProjectInit();
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("My project init is done..."));
    }
    else if (AddLoginitSuccess1)
    {

        switch (function)
        {
            // Select suitable interval for polling your function
            //    case FUNC_EVERY_SECOND:
        case FUNC_EVERY_250_MSECOND:
            //    case FUNC_EVERY_200_MSECOND:
            //    case FUNC_EVERY_100_MSECOND:
            MyProjectProcessing();
            break;

        // Command support
        case FUNC_COMMAND:
            AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("Calling My Project Command..."));
            result = DecodeCommand(MyProjectCommands, MyProjectCommand);
            break;
        }
    }

    return result;
}

#endif // USE_MY_PROJECT_CMD
