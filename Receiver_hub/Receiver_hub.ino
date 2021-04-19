#include <ESP32CAN.h>
#include <CAN_config.h>

#define WIFI_ENABLED 1
#define BUFFOR_LEN 200
#if WIFI_ENABLED
  #include <WiFi.h>
  WiFiClient client;
#endif
CAN_device_t CAN_cfg;


const char* ssid = "TP-Link_2G";
const char* password = "SebaCwel69";
const uint16_t port = 1313;
const char* host = "192.168.0.157";

void setup() {
    Serial.begin(115200);
#if WIFI_ENABLED
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
    }
    Serial.print("Connected with ip: "); Serial.println(WiFi.localIP());
    client.connect(host,port);
#endif
    Serial.println("iotsharing.com CAN demo");
    CAN_cfg.speed=CAN_SPEED_1000KBPS;
    //CAN_cfg.tx_pin_id = GPIO_NUM_5;
    //CAN_cfg.rx_pin_id = GPIO_NUM_4;
    CAN_cfg.tx_pin_id = GPIO_NUM_13;
    CAN_cfg.rx_pin_id = GPIO_NUM_12;
    CAN_cfg.rx_queue = xQueueCreate(1024,sizeof(CAN_frame_t));
    //initialize CAN Module
    ESP32Can.CANInit();
}
uint8_t dataBuffer [BUFFOR_LEN][11];
int iterator = 0;
void loop() {
    CAN_frame_t rx_frame;
    //receive next CAN frame from queue
    

    if(xQueueReceive(CAN_cfg.rx_queue,&rx_frame, 3*portTICK_PERIOD_MS)==pdTRUE){

      //do stuff!
     // if(rx_frame.FIR.B.FF==CAN_frame_std)
        //printf("New standard frame");
      //else
        //printf("New extended frame");

      if(rx_frame.FIR.B.RTR==CAN_RTR)
        ;//printf(" RTR from 0x%08x, DLC %d\r\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
      else{
        //printf(" from 0x%08x, DLC %d\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
        uint8_t dataToSend[11];
        //rx_frame.MsgID &= 0xffffffff;
        for(uint8_t i = 0; i < 4; i++) dataBuffer[iterator][i] = rx_frame.MsgID & (0xff << (24 - 8 * i));
        for(uint8_t i = 4; i < 12; i++) dataBuffer[iterator][i] = rx_frame.data.u8[i - 4];
        iterator++;
        //client.write((uint8_t *)dataToSend, 11);
        //client.flush();
      }
      if(iterator == BUFFOR_LEN)
      {
        uint8_t tempTable[BUFFOR_LEN * 11];
        for (int i = 0; i < BUFFOR_LEN; i+=11)
        {
          for (int k = 0; k < 11; k++)
          {
            tempTable[i + k] = dataBuffer[i][k];
          }
        }
        client.write((uint8_t *)tempTable, BUFFOR_LEN * 11);
        iterator = 0;
        //client.flush();

      }
      //respond to sender
      ESP32Can.CANWriteFrame(&rx_frame);
    }
}
