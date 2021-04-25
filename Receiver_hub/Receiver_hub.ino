#include <ESP32CAN.h>
#include <CAN_config.h>
#include <string.h>

#define WIFI_ENABLED 1
#define BUFFOR_LEN 10
#if WIFI_ENABLED
  #include <WiFi.h>
  WiFiClient client;
  WiFiUDP udpClient;
#endif
CAN_device_t CAN_cfg;


const char* ssid = /*"RedmiPro";*/"TP-Link_2G";
const char* password = /*"Radek$1234";*/"SebaCwel69";
const uint16_t port = 1313;
const char* host = "192.168.0.157";//"192.168.43.239";

void setup() {
    Serial.begin(115200);
#if WIFI_ENABLED
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
    }
    Serial.print("Connected with ip: "); Serial.println(WiFi.localIP());
    //client.setNoDelay(true);
    //client.connect(host,port);
    
#endif
    Serial.println("iotsharing.com CAN demo");
    CAN_cfg.speed=CAN_SPEED_1000KBPS;
    //CAN_cfg.tx_pin_id = GPIO_NUM_5;
    //CAN_cfg.rx_pin_id = GPIO_NUM_4;
    CAN_cfg.tx_pin_id = GPIO_NUM_13;
    CAN_cfg.rx_pin_id = GPIO_NUM_12;
    CAN_cfg.rx_queue = xQueueCreate(10,sizeof(CAN_frame_t));
    //initialize CAN Module
    ESP32Can.CANInit();
}
uint8_t dataBuffer [BUFFOR_LEN][12];
String buffer = "";
int iterator = 0;
void loop() {
    CAN_frame_t rx_frame;
    //receive next CAN frame from queue
    
    
    if(xQueueReceive(CAN_cfg.rx_queue,&rx_frame, 3*portTICK_PERIOD_MS)==pdTRUE){
      for(uint8_t i = 0; i < 4; i++) printf("%x ", (rx_frame.MsgID & (0xff << (24 - 8 * i))) >> (24 - 8 * i));
      printf("\n\r");
      //do stuff!
     // if(rx_frame.FIR.B.FF==CAN_frame_std)
        //printf("New standard frame");
      //else
        //printf("New extended frame");

      if(rx_frame.FIR.B.RTR==CAN_RTR)
        ;//printf(" RTR from 0x%08x, DLC %d\r\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
      else{
        //printf(" from 0x%08x, DLC %d\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);

        //rx_frame.MsgID &= 0xffffffff;
        for(uint8_t i = 0; i < 4; i++) dataBuffer[iterator][i]= (rx_frame.MsgID & (0xff << (24 - 8 * i))) >> (24 - 8 * i);
        for(uint8_t i = 4; i < 12; i++) dataBuffer[iterator][i]= rx_frame.data.u8[i - 4];
        for(int g = 0; g < 12; g++) printf("%x",dataBuffer[iterator][g]);
        //printf("\n");
        iterator++;
        //client.write((uint8_t *)dataToSend, 11);
        //client.flush();
      }
      
      if(iterator == BUFFOR_LEN)
      {
        uint8_t tempTable[BUFFOR_LEN * 15];
        memset(tempTable, 0, sizeof(tempTable));
        // TODO: Something is wrong with last byte of every frame here
        for (int i = 0; i < BUFFOR_LEN; i++)
        {
          for (int k = 0; k < 12; k++)
          {
            tempTable[i * 12 + k] = dataBuffer[i][k];
          }
        }
        //printf("CONVERTED!\n");
        //for(int g = 0; g < 1024; g++) printf("%x",tempTable[g]);
        //printf("\n");

        udpClient.beginPacket(host,port);
        for(int i = 0; i < BUFFOR_LEN * 12; i++) udpClient.write(tempTable[i]);
        udpClient.endPacket();
        
        buffer.clear();
        iterator = 0;
        //client.flush();

      }
      //respond to sender
      //ESP32Can.CANWriteFrame(&rx_frame);
    }
}
