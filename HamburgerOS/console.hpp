#pragma once

#include <iostream>
#include <string>

#include <Arduino.h>
#include <stdio.h>

using namespace std;

class Console
{
public:
    void setup();
    void print(const char *text, int8_t lvl /*= 0*/);
    void println(const char *text, int8_t lvl /*= 0*/);
    void printi(int text, int8_t lvl /*= 0*/);
    void enter();
    void read();
    void checkConfirm();
    void enterCommand();
    void clear();
    void enterCompile(std::string com);
    bool mode = false;
};