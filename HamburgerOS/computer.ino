#include "console.hpp"
#include "TimCode.hpp"
#include <EEPROM.h>

#include <Ds1302.h>
 
// Определение пинов для подключения DS1302
const int RST_PIN = 17;  // Пин для управления
const int DAT_PIN = 16;  // Пин для передачи данных
const int CLK_PIN = 15;  // Пин для синхронизации тактов
 
// Создаем экземпляр класса для работы с модулем DS1302
Ds1302 srtc(RST_PIN, CLK_PIN, DAT_PIN);

Console console;

void setup()
{
  Serial.begin(115200);

  tone(5, 600, 200);

  // Инициализация RTC
  srtc.init();
 
  // Установка началного времени и даты
  if (srtc.isHalted()) {
    Serial.println("RTC не настроен. Установка времени...");
 
    // Создание структуры DateTime с заданными значениями
    // !Установите актуальную дату и время на момент загрузки кода!
    Ds1302::DateTime dt = {
      .year = 24,
      .month = Ds1302::MONTH_SEP,
      .day = 12,
      .hour = 11,
      .minute = 51,
      .second = 30,
      .dow = Ds1302::DOW_THU
    };
 
    // Установка заданного времени и даты
    srtc.setDateTime(&dt);
  }

  console.setup();
}

void loop()
{
  console.checkConfirm();
  console.read();

  /*
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