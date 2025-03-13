#ifndef MY_LORA_E32_H
#define MY_LORA_E32_H
#ifndef ESP32
#define ESP32
#endif
#include "LoRa_E32.h"
#include "HardwareSerial.h"
// Định nghĩa chân sử dụng cho LoRa E32
#define MY_LORA_RX 7 // RX của ESP32 (gắn với TX của LoRa)
#define MY_LORA_TX 6 // TX của ESP32 (gắn với RX của LoRa)
extern char trans_data[1000];
// Khai báo đối tượng HardwareSerial (sẽ được định nghĩa trong MY_LORA_E32.cpp)
extern HardwareSerial LoraSerial;

// Khai báo đối tượng LoRa_E32 (sẽ được định nghĩa trong MY_LORA_E32.cpp)
extern LoRa_E32 my_lora_e32;

// Khai báo các hàm cấu hình và in thông số
void configMyLoraE32(uint8_t channel, uint8_t addrHigh, uint8_t addrLow, uint8_t baudRate);
void printParameters(struct Configuration configuration);
void printModuleInformation(struct ModuleInformation moduleInformation);

#endif // MY_LORA_E32_H
