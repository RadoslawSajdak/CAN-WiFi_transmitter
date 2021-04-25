#include <ESP32CAN.h>
#include <CAN_config.h>

CAN_device_t CAN_cfg;
void emulateDevice(char * data, uint32_t address, uint8_t DLC)
{
  CAN_frame_t tx_frame;

  tx_frame.FIR.B.FF = CAN_frame_std;
  tx_frame.MsgID = address;
  tx_frame.FIR.B.DLC = DLC;
  for(uint8_t i = 0; i < DLC; i++)
    tx_frame.data.u8[i] = data[i];
  
  ESP32Can.CANWriteFrame(&tx_frame);
  //delay(100); //We need to change it for sth like wait for ACK.
}
void setup() {
    Serial.begin(115200);
    Serial.println("iotsharing.com CAN demo");
    CAN_cfg.speed=CAN_SPEED_1000KBPS;
    //CAN_cfg.tx_pin_id = GPIO_NUM_13;  //ESP32-DevkitV1
    //CAN_cfg.rx_pin_id = GPIO_NUM_12;
    CAN_cfg.tx_pin_id = GPIO_NUM_5; // ESP32-CAM
    CAN_cfg.rx_pin_id = GPIO_NUM_4;
    CAN_cfg.rx_queue = xQueueCreate(100,sizeof(CAN_frame_t));
    //start CAN Module
    ESP32Can.CANInit();
}
int iterator = 0;
void loop() {
    CAN_frame_t rx_frame;
    //receive next CAN frame from queue
    if(xQueueReceive(CAN_cfg.rx_queue,&rx_frame, 3*portTICK_PERIOD_MS)==pdTRUE){

      //do stuff!
      if(rx_frame.FIR.B.FF==CAN_frame_std)
       ;// printf("New standard frame");
      else
      ;//  printf("New extended frame");

      if(rx_frame.FIR.B.RTR==CAN_RTR)
        ;//printf(" RTR from 0x%08x, DLC %d\r\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
      else{
        ;//printf(" from 0x%08x, DLC %d\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
        // for(int i = 0; i < 8; i++){
        //   printf("%c\t", (char)rx_frame.data.u8[i]);
        // }
        // printf("\n");
      }
    }
    else
    {
      
      emulateDevice(0xffeeaabbcc, 0x660, 8);
      printf("Dev2 Done!\n\r");
      iterator += 5;      
      if(iterator >= 100) while(1);
    }
}
