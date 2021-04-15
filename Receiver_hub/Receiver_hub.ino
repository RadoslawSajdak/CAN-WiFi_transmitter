#include <ESP32CAN.h>
#include <CAN_config.h>
#include <WiFi.h>

CAN_device_t CAN_cfg;
WiFiClient client;

const char* ssid = "TP-Link_2G";
const char* password = "SebaCwel69";
const uint16_t port = 1313;
const char* host = "192.168.0.157";

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
    }
    Serial.print("Connected with ip: "); Serial.println(WiFi.localIP());
    client.connect(host,port);
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

void loop() {
    CAN_frame_t rx_frame;
    //receive next CAN frame from queue
    

    if(xQueueReceive(CAN_cfg.rx_queue,&rx_frame, 3*portTICK_PERIOD_MS)==pdTRUE){

      //do stuff!
      if(rx_frame.FIR.B.FF==CAN_frame_std)
        printf("New standard frame");
      else
        printf("New extended frame");

      if(rx_frame.FIR.B.RTR==CAN_RTR)
        printf(" RTR from 0x%08x, DLC %d\r\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
      else{
        printf(" from 0x%08x, DLC %d\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
        uint8_t dataToSend[11];
        //rx_frame.MsgID &= 0xffffffff;
        for(uint8_t i = 0; i < 4; i++) dataToSend[i] = rx_frame.MsgID & (0xff << (24 - 8 * i));
        for(uint8_t i = 4; i < 12; i++) dataToSend[i] = rx_frame.data.u8[i - 4];

        client.write((uint8_t *)dataToSend, 11);
        client.flush();
        /* convert to upper case and respond to sender */
        //for(int i = 0; i < 8; i++){
          //client.write(rx_frame.data.u8[i]);
          // printf("%c", rx_frame.data.u8[i]);
          // if(rx_frame.data.u8[i] >= 'a' && rx_frame.data.u8[i] <= 'z'){
          //   rx_frame.data.u8[i] = rx_frame.data.u8[i] - 32;
          // }

        //}
        
      }
      //respond to sender
      ESP32Can.CANWriteFrame(&rx_frame);
    }
}
