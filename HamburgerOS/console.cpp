#include "console.hpp"
#include "TimCode.hpp"

#include <Ds1302.h>
 
// Определение пинов для подключения DS1302
const int RST_PIN = 17;  // Пин для управления
const int DAT_PIN = 16;  // Пин для передачи данных
const int CLK_PIN = 15;  // Пин для синхронизации тактов
 
// Создаем экземпляр класса для работы с модулем DS1302
Ds1302 rtc(RST_PIN, CLK_PIN, DAT_PIN);

#include "SPI.h"
#include "TFT_eSPI.h"

#include <Arduino.h>
#include <iostream>
#include <string>

using namespace std;

//Buzzer setup
#define BUZZER_FREQUENCY 1000
#define BUZZER_TIME 400
#define BUZZER 5

// Command buttons
#define ENTER 6
#define BACKSPACE 7

// SCREEN SETUP
TFT_eSPI tftc = TFT_eSPI();

bool mode = false;
bool out = true;

string command;

void Console::setup() {
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);

  // text size
  // setup screen
  tftc.init();
  tftc.setRotation(3);
  tftc.setTextSize(2);
  tftc.fillScreen(TFT_BLACK);
  tftc.setCursor(0, 0);
  tftc.setTextColor(TFT_WHITE);

  tftc.setTextFont(2);

  command = "info";
  enterCommand();

  delay(2000);
  tftc.fillScreen(TFT_BLACK);
  tftc.setTextSize(2);

  clear();
}

void Console::clear() {
  tftc.fillScreen(TFT_BLACK);
  tftc.setCursor(0, 0);
  if (out) {
    if (mode) {
      println("TimCode Mode", 3);
    } else {
      println("Console Mode", 3);
    }
  }
}

void Console::print(const char *text, int8_t lvl = 0) {
  Serial.print(text);
  switch (lvl) {
    case 0:
      tftc.setTextColor(TFT_WHITE);
      break;
    case 1:
      tftc.setTextColor(TFT_YELLOW);
      break;
    case 2:
      tftc.setTextColor(TFT_RED);
      break;
    case 3:
      tftc.setTextColor(TFT_GREEN);
      break;
    default:
      tftc.setTextColor(TFT_WHITE);
      break;
  }
  tftc.print(text);
  tftc.setTextColor(TFT_WHITE);
}

void Console::println(const char *text, int8_t lvl = 0) {
  Serial.println(text);
  switch (lvl) {
    case 0:
      tftc.setTextColor(TFT_WHITE);
      break;
    case 1:
      tftc.setTextColor(TFT_YELLOW);
      break;
    case 2:
      tftc.setTextColor(TFT_RED);
      break;
    case 3:
      tftc.setTextColor(TFT_GREEN);
      break;
    default:
      tftc.setTextColor(TFT_WHITE);
      break;
  }
  tftc.println(text);
  tftc.setTextColor(TFT_WHITE);
}

void Console::printi(int text, int8_t lvl = 0) {
  Serial.println(text);
  switch (lvl) {
    case 0:
      tftc.setTextColor(TFT_WHITE);
      break;
    case 1:
      tftc.setTextColor(TFT_YELLOW);
      break;
    case 2:
      tftc.setTextColor(TFT_RED);
      break;
    case 3:
      tftc.setTextColor(TFT_GREEN);
      break;
    default:
      tftc.setTextColor(TFT_WHITE);
      break;
  }
  tftc.print(text);
  tftc.setTextColor(TFT_WHITE);
}

void Console::enter() {
  Serial.println("");
  tftc.println("");
}

TimCode compiler;
string commandBuffer[196];
int commandBufferLine = 0;
int filledLines = 0;
Ds1302::DateTime dt;

void Console::read() {
  if (Serial.available()) {
    String input = Serial.readString();

    tftc.setTextColor(TFT_WHITE);

    command = command + string(input.c_str(), input.length());
    if (out) {
        if(mode){
            Serial.print(filledLines);
            tftc.print(filledLines);
            Serial.print(" "+input);
            tftc.print(" "+input);
        }
        else{
            Serial.print(input);
            tftc.print(input);
        }
    }

    command = command.substr(0, command.length() - 1);

    enterCommand();
  }
}

void Console::checkConfirm() {
  if (digitalRead(7) == LOW) {
    command = command.substr(0, command.length() - 1);
    if (out) {
      println(command.c_str());
    }
    delay(1000);
  }
}

enum Command {
  UNKNOWN,
  BEEP,
  INFO,
  MODE,
  RUN,
  ECHO,
  CLR,
  DRAWPOINT,
  DRAWLINE,
  DRAWRECT,
  FILLRECT,
  DRAWTRI,
  FILLTRI,
  DRAWCIRCLE,
  FILLCIRCLE,
  TIMESET,
  GOTO,
  NOOUT
};

void Console::enterCompile(string com) {
  command = com;
  enterCommand();
}

void Console::enterCommand() {
  if (mode) {
    if (command == string("run")) {
      clear();
      compiler.compile(commandBuffer, filledLines);
      mode = false;
    }
    if (command == string("mode")) {
      mode = !mode;
      clear();
    } else if (commandBufferLine <= 98) {
      commandBuffer[commandBufferLine] = command;
      command = "";
      commandBufferLine++;
      tone(4, 200, 100);
      enter();
      filledLines++;
    }
    if (commandBufferLine % 14 == 0 && commandBufferLine != 0) {
      tftc.fillScreen(TFT_BLACK);
      tftc.setCursor(0, 0);
      println("TimCode Mode", 3);
      if (commandBufferLine >= 98) {
        println("COMMAND BUFFER FULL!", 2);
      }
    }
  } else {
    // split args (and command) by a splitter
    string args[11];
    string splitter = " ";
    int splitter_amount = 0;
    int last_splitter_letter = 0;
    if (!isspace(command.at(command.length() - 1))) {
      command = command + " ";
    }
    for (size_t i = 0; i < command.length(); ++i) {
      if (isspace(command.at(i))) {
        args[splitter_amount] = command.substr(last_splitter_letter, i);
        last_splitter_letter = i;
        splitter_amount++;
        // print(" s ");
      }
      // print(" t ");
    }
    if (splitter_amount == 0) {
      args[0] = command;
    }
    string commandf = args[0];

    int args_length = 0;
    for (size_t i = 0; i < command.length(); ++i) {
      if (isspace(command.at(i))) {
        args_length++;
      }
    }
    // printi(args_length, 3);

    /*
        println(args[0].c_str());
        for (size_t i = 0; i < args_length; i++)
        {
            println(args[i].c_str(), 1);
        }
        */

    // convert string to enum
    int currentCommand;
    if (commandf == string("beep")) {
      currentCommand = BEEP;
    } else if (commandf == string("info")) {
      currentCommand = INFO;
    } else if (commandf == string("mode")) {
      currentCommand = MODE;
    } else if (commandf == string("run")) {
      currentCommand = RUN;
    } else if (commandf == string("echo")) {
      currentCommand = ECHO;
    } else if (commandf == string("clr")) {
      currentCommand = CLR;
    } else if (commandf == string("drawpoint")) {
      currentCommand = DRAWPOINT;
    } else if (commandf == string("drawline")) {
      currentCommand = DRAWLINE;
    } else if (commandf == string("noout")) {
      currentCommand = NOOUT;
    } else if (commandf == string("drawrect")) {
      currentCommand = DRAWRECT;
    } else if (commandf == string("fillrect")) {
      currentCommand = FILLRECT;
    } else if (commandf == string("drawtri")) {
      currentCommand = DRAWTRI;
    } else if (commandf == string("filltri")) {
      currentCommand = FILLTRI;
    }
    else if(commandf == string("drawcircle")){
        currentCommand = DRAWCIRCLE;
    }
    else if(commandf == string("fillcircle")){
        currentCommand = FILLCIRCLE;
    }
    else if(commandf == string("timeset")){
        currentCommand = TIMESET;
    }
    else if(commandf == string("goto")){
        currentCommand = GOTO;
    }
     else {
      currentCommand = UNKNOWN;
    }

    // run command
    enter();

    switch (currentCommand) {
      case UNKNOWN:
        if (out) {
          println("UNKNOWN", 2);
        }
        break;

      case BEEP:
        if (args_length > 0) {
          if (args_length == 2) {
            tone(BUZZER, stoi(args[1]));
          } else if (args_length == 3) {
            tone(BUZZER, stoi(args[1]), stoi(args[2]));
          }
        } else {
          tone(BUZZER, BUZZER_FREQUENCY, BUZZER_TIME);
        }
        break;

      case INFO:
        if (out) {
          println("_===_  HamburgerOS", 1);
          println("-_-_-  By TimiksOMG", 3);
          println("=====", 2);
          println("-===-  c 2025", 1);
          enter();
          
          Ds1302::DateTime now;
          rtc.getDateTime(&now);

          print("Time: ");
          printi(now.hour);
          print(":");
          printi(now.minute);
          print(":");
          printi(now.second);
          enter();

          print("Date: ");
          printi(now.day);
          print(".");
          printi(now.month);
          print(".");
          printi(now.year);
          enter();
        }
        break;
      case MODE:
        mode = !mode;
        clear();
        break;
      case RUN:
        compiler.compile(commandBuffer, filledLines);
        break;
      case ECHO:
        print(command.substr(5, command.length()).c_str());
        break;
      case CLR:
        clear();
        break;
      case DRAWPOINT:
        tftc.drawPixel(stoi(args[1]), stoi(args[2]), tftc.color565(stoi(args[3]), stoi(args[4]), stoi(args[5])), stoi(args[6]));
        break;
      case DRAWLINE:
        tftc.drawLine(stoi(args[1]), stoi(args[2]), stoi(args[3]), stoi(args[4]), tftc.color565(stoi(args[5]), stoi(args[6]), stoi(args[7])));
        break;
      case DRAWRECT:
        tftc.drawRect(stoi(args[1]), stoi(args[2]), stoi(args[3]), stoi(args[4]), tftc.color565(stoi(args[5]), stoi(args[6]), stoi(args[7])));
        break;
      case FILLRECT:
        tftc.fillRect(stoi(args[1]), stoi(args[2]), stoi(args[3]), stoi(args[4]), tftc.color565(stoi(args[5]), stoi(args[6]), stoi(args[7])));
        break;
      case DRAWTRI:
        tftc.drawTriangle(stoi(args[1]), stoi(args[2]), stoi(args[3]), stoi(args[4]), stoi(args[5]), stoi(args[6]), tftc.color565(stoi(args[7]), stoi(args[8]), stoi(args[9])));
        break;
      case FILLTRI:
        tftc.fillTriangle(stoi(args[1]), stoi(args[2]), stoi(args[3]), stoi(args[4]), stoi(args[5]), stoi(args[6]), tftc.color565(stoi(args[7]), stoi(args[8]), stoi(args[9])));
        break;
      case DRAWCIRCLE:
        tftc.drawCircle(stoi(args[1]), stoi(args[2]), stoi(args[3]), tftc.color565(stoi(args[4]), stoi(args[5]), stoi(args[6])));
        break;
      case FILLCIRCLE:
        tftc.fillCircle(stoi(args[1]), stoi(args[2]), stoi(args[3]), tftc.color565(stoi(args[4]), stoi(args[5]), stoi(args[6])));
        break;
      case TIMESET:
        dt = {
        .year = stoi(args[6]),
        .month = stoi(args[5]),
        .day = stoi(args[4]),
        .hour = stoi(args[3]),
        .minute = stoi(args[2]),
        .second = stoi(args[1]),
        .dow = Ds1302::DOW_THU
        };
    
        // Установка заданного времени и даты
        rtc.setDateTime(&dt);
        break;
      case GOTO:
        compiler.change = true;
        compiler.progress = stoi(args[1]);
        break;
      case NOOUT:
        out = !out;
        break;
      default:
        tone(BUZZER, 200, 2000);
        println("UNKNOWN", 2);
        break;
    }

    command = "";
    enter();
  }
}