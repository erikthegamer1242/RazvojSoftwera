// #include <lvgl.h>
// #include <Arduino.h>
// #include <TFT_eSPI.h>
// #include "FT6236.h"
// #include <map>
// #include <SPI.h>
// #include <utility>
// #include <LoRa.h>
// #include "LoRaMesh.h"
// #include <Arduino.h>
// #include <FS.h>
// #include <SPIFFS.h>
// #include "msgCommon.h"
// #include <ui.h>
// #include <TinyGPSPlus.h>
// #include <SoftwareSerial.h>

// extern int myId;
// int idToSend = 0; 
// int pushButtonPin = 36;
// int battVoltagePin = 35;
// int battPresentage = 100;
// std::vector<lv_obj_t *> msgElements;
// unsigned long startT;
// unsigned long currT;
// unsigned long endT = 10e3;
// unsigned long screanSaverCurrT;
// unsigned long screanSaverLastT;
// unsigned long screanSaverEndT;
// uint8_t touch = 10;
// int8_t sda = 21;
// int8_t scl = 22;
// static const int RXPin = 34, TXPin = 0;
// static const uint32_t GPSBaud = 9600;

// FT6236 ts = FT6236();
// TinyGPSPlus gps;
// LoRaMesh loramesh;
// SoftwareSerial ss(RXPin, TXPin);

// TaskHandle_t Task1;
// TaskHandle_t Task2;

// /*Don't forget to set Sketchbook location in File/Preferencesto the path of your UI project (the parent foder of this INO file)*/

// /*Change to your screen resolution*/
// static const uint16_t screenWidth  = 320;
// static const uint16_t screenHeight = 480;

// static lv_disp_draw_buf_t draw_buf;
// static lv_color_t buf[ screenWidth * screenHeight / 10 ];

// TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

// #if LV_USE_LOG != 0
// /* Serial debugging */
// void my_print(const char * buf)
// {
//     Serial.printf(buf);
//     Serial.flush();
// }
// #endif

// /* Display flushing =*/
// void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
// {
//     uint32_t w = ( area->x2 - area->x1 + 1 );
//     uint32_t h = ( area->y2 - area->y1 + 1 );

//     tft.startWrite();
//     tft.setAddrWindow( area->x1, area->y1, w, h );
//     tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
//     tft.endWrite();

//     lv_disp_flush_ready( disp );
// }

// /*Read the touchpad*/
// void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
// {
//     TS_Point p;
//     bool istouched = ts.touched();
//     if (istouched) {
//         screanSaverLastT = millis();
//         // Retrieve a point
//         p = ts.getPoint();

//         data->point.x = map(p.x, 0, 319, 319, 0);
//         data->point.y = map(p.y, 0, 479, 479, 0);
//         if (p.y != 0 && p.x != 0) data->state = LV_INDEV_STATE_PRESSED;
//         else data->state = LV_INDEV_STATE_RELEASED;

//     } else {
//         data->state = LV_INDEV_STATE_RELEASED;
//     }
// }

// void updateBatt(){
//   Serial.printf("updateBatt()\n");
//   battPresentage =  analogRead(battVoltagePin);
//   Serial.printf("battPresentage = %d\n", battPresentage);
//   battPresentage = map(battPresentage, 0, 4095, 0, 100);
//   Serial.printf(" map() = %d\n", battPresentage);
// 	lv_label_set_text(ui_BatteryPercentage2, ("Battery: " + String(battPresentage) + "%").c_str());
//   lv_label_set_text(ui_BatteryPercentage1, ("Battery: " + String(battPresentage) + "%").c_str());
// }

// void setup()
// {
//   Serial.println("setup()");
//   Serial.print("Setup: Executing on core ");
//   Serial.println(xPortGetCoreID());

//   pinMode(32, OUTPUT);
//   pinMode(26, OUTPUT);
//   digitalWrite(32, HIGH);
//   digitalWrite(26, HIGH);
//   delay(1000);

//   screanSaverLastT = millis();
//   pinMode(pushButtonPin, INPUT);
//   pinMode(battVoltagePin, INPUT);
//   Serial.begin(115200); /* prepare for possible serial debug */
//   ss.begin(9600);

//   if(!SPIFFS.begin(1)){
//       Serial.println("SPIFFS Failed!");
//       return;
//   }
 
//   ts.begin(touch, sda, scl) ;
//   String LVGL_Arduino = "Hello Arduino! ";
//   LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
//   Serial.println(LVGL_Arduino);

//   lv_init();
//   Serial.print("lv_init(");
//   Serial.print("");
//   Serial.println(") Success!");

//   #if LV_USE_LOG != 0
//     lv_log_register_print_cb(my_print); /* register print function for debugging */
//   #endif

//   tft.begin(); // TFT init
//   tft.setRotation(2); // Landscape orientation, flipped

//   lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

//   /* Initialize the display */
//   static lv_disp_drv_t disp_drv;
//   lv_disp_drv_init(&disp_drv);
//   /*Change the following line to your display resolution */
//   disp_drv.hor_res = screenWidth;
//   disp_drv.ver_res = screenHeight;
//   disp_drv.flush_cb = my_disp_flush;
//   disp_drv.draw_buf = &draw_buf;
//   lv_disp_drv_register(&disp_drv);

//   /* Initialize the (dummy) input device driver */
//   static lv_indev_drv_t indev_drv;
//   lv_indev_drv_init(&indev_drv);
//   indev_drv.type = LV_INDEV_TYPE_POINTER;
//   indev_drv.read_cb = my_touchpad_read;
//   lv_indev_drv_register(&indev_drv);

//   ui_init();
//   Serial.print("ui_init(");
//   Serial.print("");
//   Serial.println(") Success!");
//   delay(500);
  
//   startT = millis();
//   endT = 10e3;
//   screanSaverEndT = 600e3;

//   Serial.printf("gps.location.lat() = %llu", gps.location.lat());
//   Serial.printf("gps.location.lng() = %llu", gps.location.lng());
//   // loraMesh more biti zadnji u setup() funkciji
//   loramesh.loraSetup(myId);
//   updateBatt();
  
//   Serial.print("setup(");
//   Serial.print("");
//   Serial.println(") Success!");
// }


// void loop(){
//     currT = millis();
//     screanSaverCurrT = millis();
//     int packetSize = loramesh.parsePacket();
//     if(packetSize){
//       loramesh.meshRead();  
//     }
//     if(currT - startT >= endT){
//       startT = millis(); 
//       endT = 60e3; // MAKNUTI
//       Serial.println("Timeout!");
//       loramesh.helloPacket(loramesh.createRoutingPacket());
//       updateBatt();
//     }
//     if(digitalRead(pushButtonPin) == HIGH){
//       digitalWrite(32,LOW);
//       while(1);
//     } 

//     if(screanSaverCurrT - screanSaverLastT >= screanSaverEndT){ 
//       digitalWrite(26, LOW);
//       // Serial.printf("Display turn off\n");
//     }
//     else{
//       digitalWrite(26, HIGH);
//       // Serial.printf("Display turn on\n");
//     }
//     lv_timer_handler();
//     delay(5);
// }
