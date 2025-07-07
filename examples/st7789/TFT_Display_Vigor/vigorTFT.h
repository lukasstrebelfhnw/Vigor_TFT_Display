//
// Created by lukas on 05.03.2025.
//

#ifndef VIGORTFT_H
#define VIGORTFT_H

#include "vigorTFT.h"
#include <ctime>
#include <chrono>
#include <thread>
#include <string>
#include <iostream>
#include <cstring>
#include "ST7789_TFT_LCD_RVL.hpp"
#include <hiredis/hiredis.h>
#include <unordered_map>
#include "TextBox.h"

// Color definitions 16-Bit Color Values R5G6B5 from Vigor Frontpaneel
#define buttonGrey 0x52EC
#define buttonAuto 0x1389
#define buttonSemi 0xB666
#define buttonMan 0xFE88
#define buttonRand 0xFFE0
#define backGroundColor 0x0000
#define vigorDGreen 0x73E6
#define vigorLGreen 0xADE6

// Datatype definitions
using message = std::string;
using signalGPS = std::string;
using objectName = std::string;
using mode = std::string;
using number = double;
using text = std::string;

class vigorTFT : public ST7789_TFT
{
private:
public:
  // Constructor and Destructor
  vigorTFT();
  ~vigorTFT();

  // Creates Display Methods
  void createInitDisplay(uint16_t bitMapWidth, uint16_t bitMapHeight, const char *path, std::string versionVigor, uint16_t myTFTHeight, uint16_t myTFTWidth);
  void createDisplay(const std::unordered_map<std::string, std::string> &data, const std::unordered_map<std::string, TextBox> &textBoxes, const std::string &currentState);
  void createLoadingBar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t lineThickness, uint16_t colorBackgroung, uint16_t colorFrame, uint16_t colorBar, uint16_t barValue, bool showValue);

  // Set Bitmap Picture
  void drawBMPPicture(uint16_t x, uint16_t y, uint16_t bitMapWidth, uint16_t bitMapHeight, const char *path); // Check Parameters, Height, Width, Color, File
  void drawText(const TextBox &box, const std::string &text);
};

#endif // VIGORTFT_H