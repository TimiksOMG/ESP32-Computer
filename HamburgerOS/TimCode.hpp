#ifndef TIMCODE_H
#define TIMCODE_H

#pragma once

#include "iostream"
#include "string"
#include <stdio.h>
using namespace std;

class TimCode
{
public:
    int progress;
    bool change;
    void compile(string commandBuffer[], int lines);
};

#endif