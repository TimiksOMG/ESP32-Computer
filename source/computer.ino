#include "console.hpp"

//rtc setup
#include <Ds1302.h>
const int RST_PIN = 17;  // Пин для управления
const int DAT_PIN = 16;  // Пин для передачи данных
const int CLK_PIN = 15;  // Пин для синхронизации тактов
Ds1302 srtc(RST_PIN, CLK_PIN, DAT_PIN);

void setup()
{
  //setup the serial port
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  //initialize rtc
  srtc.init();
 
  //initialize rtc rime
  if (srtc.isHalted()) {
    Serial.println("RTC не настроен. Установка времени...");
 
    Ds1302::DateTime dt = {
      .year = 25,
      .month = 8,
      .day = 29,
      .hour = 20,
      .minute = 57,
      .second = 15,
    };
 
    //set
    srtc.setDateTime(&dt);
  }

  //start the console setup
  console::setup();
}

void loop()
{
  //console check
  console::read();

  /*
  //tried something with eeprom
  if(millis / 1000 % EEPROM.read(0) * 6000 == 0){
    EEPROM.begin(2);
    if(EEPROM.read(1) == 255){
      EEPROM.write(1, 0);
      EEPROM.write(0, EEPROM.read(0) + 1);
    }
    EEPROM.write(1, (uint8_t)millis / EEPROM.read(0) * (uint8_t)60000);
    EEPROM.commit();
    EEPROM.end();
  }
  */
}