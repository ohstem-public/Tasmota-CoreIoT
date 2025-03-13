#include "MYLORA_E32.h"
HardwareSerial LoraSerial(1);
LoRa_E32 my_lora_e32(MY_LORA_TX, MY_LORA_RX, &LoraSerial, UART_BPS_RATE_9600, SERIAL_8N1);
void configMyLoraE32(uint8_t channel, uint8_t addrHigh, uint8_t addrLow, uint8_t baudRate)
{
    // Lấy cấu hình hiện tại của module
    ResponseStructContainer c = my_lora_e32.getConfiguration();
    if (c.data == NULL)
    {
        return;
    }
    Configuration configuration = *(Configuration *)c.data;

    // Cập nhật các thông số với giá trị truyền vào
    configuration.ADDH = addrHigh; // Địa chỉ cao
    configuration.ADDL = addrLow;  // Địa chỉ thấp
    configuration.CHAN = channel;  // Kênh truyền
    configuration.SPED.uartParity = 0;           // 8N1
    configuration.SPED.uartBaudRate = baudRate;  // Tốc độ baud UART
    configuration.SPED.airDataRate = 2;          // Tốc độ truyền không khí mặc định
    configuration.OPTION.fixedTransmission = 0;  // Chế độ Transparent
    configuration.OPTION.ioDriveMode = 1;
    configuration.OPTION.wirelessWakeupTime = 3; // Wakeup Time mặc định
    configuration.OPTION.fec = 1;
    configuration.OPTION.transmissionPower = 0;
    // Gửi cấu hình mới đến module và lưu lại cấu hình
    
    my_lora_e32.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
    ResponseStructContainer cMi;
    cMi = my_lora_e32.getModuleInformation();
    ModuleInformation mi = *(ModuleInformation *)cMi.data;
    // In ra Serial để kiểm tra
    c.close();
}
