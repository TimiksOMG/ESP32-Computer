//other classes
#include "console.hpp"
#include "storage.hpp"

//libraries
#include <Ds1302.h>
#include "SPI.h"
#include "TFT_eSPI.h"
#include <Arduino.h>
#include <iostream>
#include <string>
 
 
//creating an rtc (again)
const int RST_PIN = 17;  // Пин для управления
const int DAT_PIN = 16;  // Пин для передачи данных
const int CLK_PIN = 15;  // Пин для синхронизации тактов
Ds1302 rtc(RST_PIN, CLK_PIN, DAT_PIN);

//namespaces
using namespace console;
using namespace storage;
using namespace std;

//Buzzer setup (default values)
#define BUZZER_FREQUENCY 1000
#define BUZZER_TIME 400
#define BUZZER 6


//command buffer (max length of a timcode program)
#define COMMAND_BUFFER_LENGTH 196


namespace console{
//screen object
TFT_eSPI tftc = TFT_eSPI();

//some console parameters
bool out = true;
bool mode = false;
string command;

void setup() {
  //buzzer setup
  pinMode(BUZZER, OUTPUT);

  // text size
  // setup screen
  tftc.init();
  tftc.setRotation(3);
  tftc.setTextSize(1);
  tftc.setTextFont(2);

  //clear screen
  tftc.fillScreen(TFT_BLACK);
  tftc.setCursor(0, 0);
  tftc.setTextColor(TFT_WHITE);

  //display info (uses console ;) )
  command = "info";
  enterCommand();

  //wait and start
  delay(1200);
  tftc.fillScreen(TFT_BLACK);
  clear();
}

//clear the screen and display current mode (if out is enabled)
void clear() {
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

//prints a char* to Serial and the screen
void print(const char *text, int8_t lvl = 0) {
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

//prints a char* to Serial and the screen and goes to next 
void println(const char *text, int8_t lvl = 0) {
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

//prints prints an int to Serial and the screen
void printi(int text, int8_t lvl = 0) {
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

//goes to the next line
void enter() {
  Serial.println("");
  tftc.println("");
}

//TimCode setup
string commandBuffer[196];
int commandBufferLine = 0;
int filledLines = 0;
int progress;
bool change;
bool noout;

//to set up time via command
Ds1302::DateTime dt;

//reads a string from Serial
void read() {
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
            Serial.print("run: ");
            tftc.print("run: ");
            Serial.print(input);
            tftc.print(input);
        }
    }

    command = command.substr(0, command.length() - 1);

    if(command == string("break")){
      change = true;
      progress = filledLines + 1;
    }

    enterCommand();
  }
}

//runs the TimCode buffer
void compile(string commandBuffer[COMMAND_BUFFER_LENGTH], int lines){
  for (int i; i < lines; i++)
    { 
        command = commandBuffer[i];
        enterCommand();
        if(change){
            i = progress;
            change = false;
        }
    }
    progress = 0;
    enter();

    printi(lines,2);
    println("Compilation finished!", 3);
}

//Then you press enter
void enterCommand() {
  if (mode) {
    //all this runs if youre in TimCode mode
    if (command == string("run")) {
      clear();
      mode = false;
      compile(commandBuffer, filledLines);
    }
    if (command == string("mode")) {
      mode = !mode;
      clear();
    } else if (commandBufferLine <= COMMAND_BUFFER_LENGTH) {
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
      if (commandBufferLine >= COMMAND_BUFFER_LENGTH) {
        println("COMMAND BUFFER FULL!", 2);
      }
    }
  } else {
    //this runs in Console mode
    //i ain't documenting all this

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

    args[1].erase(0, 1);

    args[2].erase(0, 1);

    // printi(args_length, 3);

    /*
        println(args[0].c_str());
        for (size_t i = 0; i < args_length; i++)
        {
            println(args[i].c_str(), 1);
        }
        */
    
    if (commandf == string("beep")) {
      if (args_length > 0) {
          if (args_length == 2) {
            tone(BUZZER, stoi(args[1]));
          } else if (args_length == 3) {
            tone(BUZZER, stoi(args[1]), stoi(args[2]));
          }
        } else {
          tone(BUZZER, BUZZER_FREQUENCY, BUZZER_TIME);
        }
    } else if (commandf == string("info")) {
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
     }
    else if(commandf == string("wait")){
      console::println("wait!");
      delay(stoi(args[1]));
    } else if (commandf == string("mode")) {
        mode = !mode;
        clear();
      compile(commandBuffer, filledLines);
    } else if (commandf == string("echo")) {
      tftc.setCursor(0, stoi(args[1]));
        print(command.substr(8, command.length()).c_str());
    } else if (commandf == string("clr")) {
      clear();
    } else if (commandf == string("drawpoint")) {
      tftc.drawPixel(stoi(args[1]), stoi(args[2]), tftc.color565(stoi(args[3]), stoi(args[4]), stoi(args[5])), stoi(args[6]));
    } else if (commandf == string("drawline")) {
      tftc.drawLine(stoi(args[1]), stoi(args[2]), stoi(args[3]), stoi(args[4]), tftc.color565(stoi(args[5]), stoi(args[6]), stoi(args[7])));
    } else if (commandf == string("drawrect")) {
      tftc.drawRect(stoi(args[1]), stoi(args[2]), stoi(args[3]), stoi(args[4]), tftc.color565(stoi(args[5]), stoi(args[6]), stoi(args[7])));
    } else if (commandf == string("fillrect")) {
      tftc.fillRect(stoi(args[1]), stoi(args[2]), stoi(args[3]), stoi(args[4]), tftc.color565(stoi(args[5]), stoi(args[6]), stoi(args[7])));
    } else if (commandf == string("drawtri")) {
      tftc.drawTriangle(stoi(args[1]), stoi(args[2]), stoi(args[3]), stoi(args[4]), stoi(args[5]), stoi(args[6]), tftc.color565(stoi(args[7]), stoi(args[8]), stoi(args[9])));
    } else if (commandf == string("filltri")) {
      tftc.fillTriangle(stoi(args[1]), stoi(args[2]), stoi(args[3]), stoi(args[4]), stoi(args[5]), stoi(args[6]), tftc.color565(stoi(args[7]), stoi(args[8]), stoi(args[9])));
      }
    else if(commandf == string("drawcircle")){
      tftc.drawCircle(stoi(args[1]), stoi(args[2]), stoi(args[3]), tftc.color565(stoi(args[4]), stoi(args[5]), stoi(args[6])));
      }
    else if(commandf == string("fillcircle")){
      tftc.fillCircle(stoi(args[1]), stoi(args[2]), stoi(args[3]), tftc.color565(stoi(args[4]), stoi(args[5]), stoi(args[6])));
      }
    else if(commandf == string("timeset")){
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
     }
    else if(commandf == string("goto")){
        change = true;
        progress = stoi(args[1]);
     }
    else if(commandf == string("noout")){
      out = !out;
     }
    else if(commandf == string("sdlist")){
        storage::listDir("/", 0);
     }
     else if(commandf == string("sdstart")){
        storage::begin();
     }
    else if(commandf == string("sdmkdir")){
      storage::createDir(args[1].c_str());
    }
    else if(commandf == string("sdrmdir")){
      storage::removeDir(args[1].c_str());
    }
    else if(commandf == string("sdopen")){
      storage::readFile(args[1].c_str());
    }
    else if(commandf == string("sdwrite")){
      storage::writeFile(args[1].c_str(), args[2].c_str());
    }
    else if(commandf == string("sdappend")){
      storage::appendFile(args[1].c_str(), args[2].c_str());
    }
    else if(commandf == string("sdrename")){
      storage::renameFile(args[1].c_str(), args[2].c_str());
    }
    else if(commandf == string("sdremove")){
      storage::deleteFile(args[1].c_str());
    }
    else if(commandf == string("reset")){
      ESP.restart(); // Software reset
    }
    else {
    if (out) {
        print(commandf.c_str(), 2);
        println(": UNKNOWN", 2);
      }
    }

    command = "";
    enter();
  }
}
}