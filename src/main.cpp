#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HARestAPI.h>
#include <ArduinoJson.h>

#include <U8g2lib.h>
#include <Arduino_GFX_Library.h>
//#include <lvgl.h>

// enable or disable debug mode to serial console
#define DEBUG

#ifdef DEBUG
#define TRACE(x) Serial.println(x);
#else
#define TRACE(x)
#endif


Arduino_DataBus *bus = new Arduino_ESP32SPI(19 /* DC */, 5 /* CS */, 18 /* SCK */, 23 /* MOSI */, GFX_NOT_DEFINED /* MISO */, VSPI /* spi_num */);
//Arduino_GFX *gfx = new Arduino_GC9A01(bus, 2 /*DF_GFX_RST*/, 0 /* rotation */, true /* IPS */);

Arduino_G *output_display  = new Arduino_GC9A01(bus, 2 /*DF_GFX_RST*/, 0 /* rotation */, true /* IPS */);
Arduino_GFX *gfx = new Arduino_Canvas(240 /* width */, 239 /* height */, output_display);

/* Change to your screen resolution */
//static uint32_t screenWidth;
//static uint32_t screenHeight;
//static lv_disp_draw_buf_t draw_buf;
//static lv_color_t *disp_draw_buf;
//static lv_disp_drv_t disp_drv;

WiFiClient sclient;
HARestAPI ha(sclient);

// Network Settings
const char* ssid = "Bommel";
const char* password = "HetWachtwoordIsFizbo";
/* Put StaticIP Address details */
//IPAddress local_ip(192.168.1.1); // Set static IP
//IPAddress gateway(192.168.1.1); // Set network Gateway IP
//IPAddress subnet(255, 255, 255, 0); // Set static IP
//IPAddress primaryDNS(8, 8, 8, 8);   //optional
//IPAddress secondaryDNS(8, 8, 4, 4); //optional

#define RETRY_ATTEMPTS 5 // Number of retrys to connect to WiFi
// Home Assistant IP and token key
const char* ha_ip = "192.168.0.185";
uint16_t ha_port = 8123; // Could be 443 is using SSL
const char* ha_pwd = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiI0ZTE1MzM2MjY4MTI0YjRmYjkxZTc2ODFlNWI0MzYwYyIsImlhdCI6MTY2NjI4NjE3MCwiZXhwIjoxOTgxNjQ2MTcwfQ.LP_bfasiiClLF5QHPQET0XWsW2U1vxxnUh1MgvcBi6w";  //long-lived password. On HA, Profile > Long-Lived Access Tokens > Create Token

StaticJsonDocument<512> doc;
float values[11];
int valueCounter = 0;

int updateCounter = 100;

/* Display flushing */
//void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
//{
//   uint32_t w = (area->x2 - area->x1 + 1);
//   uint32_t h = (area->y2 - area->y1 + 1);

//   gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);

//   lv_disp_flush_ready(disp);
//}

float ParseSensorJsonData(String strInput) {
  TRACE(strInput);
  DeserializationError error = deserializeJson(doc, strInput);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
  } else {
    const char* state = doc["state"];
    return atof(state);
  }
  return 0;
}

void setupHA()
{
     // Start Home Assistant rest api data
  ha.setHAServer(ha_ip, ha_port);
  ha.setHAPassword(ha_pwd);
  ha.setDebugMode(false);
  

}

float ReadValue()
{
  float fltLiving = ParseSensorJsonData( ha.sendGetHA("/api/states/sensor.lumi_lumi_weather_temperature"));
  //String val = ha.sendGetHA("/api/states/sensor.hot_water_top");
  TRACE("Living Temp: ");
  TRACE(fltLiving);

  return fltLiving;
}

void setupWifi()
{
  // Connect to WiFi
  TRACE("\nConnecting to Wifi\n");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
}

//static lv_obj_t * meter;
//static lv_meter_indicator_t * indic;
//static lv_obj_t * labelTemp;

//static void set_value(void * indic2, int32_t v)
//{
//   lv_meter_set_indicator_end_value(meter, indic, v);
//}

/**
 * A simple meter
 */
//void lv_example_meter_1(void)
//{
//    meter = lv_meter_create(lv_scr_act());
//    lv_obj_center(meter);
//    lv_obj_set_size(meter, 260, 260);

    /*Add a scale first*/
//    lv_meter_scale_t * scale = lv_meter_add_scale(meter);
//    lv_meter_set_scale_ticks(meter, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
//    lv_meter_set_scale_major_ticks(meter, scale, 8, 4, 15, lv_color_black(), 10);

    /*Add a blue arc to the start*/
//    indic = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_BLUE), 0);
//    lv_meter_set_indicator_start_value(meter, indic, 0);
//    lv_meter_set_indicator_end_value(meter, indic, 20);

    /*Make the tick lines blue at the start of the scale*/
//    indic = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_BLUE),
//                                     false, 0);
//    lv_meter_set_indicator_start_value(meter, indic, 0);
//    lv_meter_set_indicator_end_value(meter, indic, 20);

    /*Add a red arc to the end*/
//    indic = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
//    lv_meter_set_indicator_start_value(meter, indic, 80);
//    lv_meter_set_indicator_end_value(meter, indic, 100);

    /*Make the tick lines red at the end of the scale*/
//    indic = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false,
//                                     0);
//    lv_meter_set_indicator_start_value(meter, indic, 80);
//    lv_meter_set_indicator_end_value(meter, indic, 100);

    /*Add a needle line indicator*/
//    indic = lv_meter_add_needle_line(meter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);
    //indic = lv_meter_add_arc(meter, scale, 20, lv_palette_main(LV_PALETTE_BLUE), 20);
    
    /*Remove the circle from the middle*/
//    lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);
    
    

    /*Create an animation to set the value*/
    /*lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_value);
    lv_anim_set_var(&a, indic);
    lv_anim_set_values(&a, 0, 100);
    lv_anim_set_time(&a, 2000);
    lv_anim_set_repeat_delay(&a, 100);
    lv_anim_set_playback_time(&a, 500);
    lv_anim_set_playback_delay(&a, 100);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);*/
//}

//void Temperature(void)
//{
//   labelTemp = lv_label_create(lv_scr_act());
//   lv_label_set_recolor(labelTemp, true);
   
//   lv_obj_center(labelTemp);
//}

void setup()
{
   Serial.begin(115200);
   // while (!Serial);
   Serial.println("LVGL Hello World");

   // Init Display
   gfx->begin();
   gfx->fillScreen(BLACK);
gfx->setUTF8Print(true);
   //lv_init();

   //screenWidth = gfx->width();
   //screenHeight = gfx->height();

   //disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * 10, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

      //lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * 10);

      /* Initialize the display */
      //lv_disp_drv_init(&disp_drv);
      /* Change the following line to your display resolution */
      //disp_drv.hor_res = screenWidth;
      //disp_drv.ver_res = screenHeight;
      //disp_drv.flush_cb = my_disp_flush;
      //disp_drv.draw_buf = &draw_buf;
      //lv_disp_drv_register(&disp_drv);

      /* Initialize the (dummy) input device driver */
      //static lv_indev_drv_t indev_drv;
      //lv_indev_drv_init(&indev_drv);
      //indev_drv.type = LV_INDEV_TYPE_POINTER;
      //lv_indev_drv_register(&indev_drv);

      //lv_example_meter_1();
      //Temperature();
     
      Serial.println("Setup done");

      setupWifi();
      setupHA();
}

int tempToY(float temp)
{
   temp = (temp * 8) - 80; //10 is 0 -- 30 is max
   return 240 - temp;
}

void loop()
{
      gfx->fillScreen(BLACK);
   //lv_timer_handler(); /* let the GUI do its work */

   float value = ReadValue();

//every 30min
   if (updateCounter++ > 30) 
   {
      values[valueCounter++] = value;
      if (valueCounter > 10) valueCounter = 0;

      Serial.println("Value added");
      updateCounter = 0;
   }

   //lv_meter_set_indicator_end_value(meter, indic, value);
   char buffer[40];
   sprintf(buffer, "%.1f°C", value);

   float oldY = tempToY(values[0]);
   int oldX=0;
   
   /*
   for(int i = 1; i< 11;i++)
   {
      float newY = tempToY(values[i]);
      int newX = i*24;

      Serial.printf("%d: (%d, %.1f)-(%d, %.1f)", i, oldX, oldY, newX, newY);
      Serial.println();



      //gfx->drawLine(oldx,oldvalue, i*24,values[i]+120,BLUE);
      gfx->fillTriangle(oldX,oldY, newX,newY, oldX, 240, BLUE);
      gfx->fillTriangle(newX,newY, oldX, 240, newX, 240, BLUE);

      oldX=i*24;
      oldY=tempToY(values[i]);
   }
   */

   //lv_label_set_text(labelTemp, buffer);

gfx->setCursor(25, 150);
gfx->setTextColor(WHITE);
//gfx->setTextSize(5);
gfx->setFont(u8g2_font_logisoso58_tf);
gfx->printf(buffer);

 gfx->flush();
   delay(5000);
}