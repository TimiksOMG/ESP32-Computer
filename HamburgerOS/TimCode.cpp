#include "TimCode.hpp"

#include <Arduino.h>
#include <console.hpp>
#include "string"
#include <stdio.h>
using namespace std;

#define BUZZER_FREQUENCY 600
#define BUZZER_TIME 200

Console tconsole;

int progress;
bool change;

void TimCode::compile(string commandBuffer[196], int lines){
    for (int i; i < lines; i++)
    { 
        //Serial.println(i);
        tconsole.enterCompile(commandBuffer[i]);
        //if(!change){
            //i = progress;
            //change = true;
        //}
    }
    //progress = 0;
    tconsole.enter();

    tconsole.println("Compilation finished!", 3);
}