#ifndef MAX4466_H
#define MAX4466_H

#include <Arduino.h>
#include <Wire.h>

constexpr int micPin{ 34 };

/**
 * @brief Initialize microphone max 4466
 * 
 */
void micInit();

/**
 * @brief Listen room noise
 * 
 */
void micReadData();

#endif
