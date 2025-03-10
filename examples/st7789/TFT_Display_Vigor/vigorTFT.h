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
#include "ST7789_TFT_LCD_RVL.hpp"
#include "Bi_Color_Bitmap_Data.hpp"
/*
#include "font_data_RVL.hpp"
#include "print_data_RVL.hpp"
*/

// Color definitions 16-Bit Color Values R5G6B5 from Vigor Frontpaneel
#define buttonGrey 0x52EC
#define buttonAuto 0x1389
#define buttonSemi 0xB666
#define buttonMan 0xFE88
#define buttonRand 0x543A
#define backGroundColor 0x9D14
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
  void createInitDisplay(uint16_t bitMapWidth, uint16_t bitMapHeight, const char *path, const char *versionVigor, uint16_t myTFTHeight, uint16_t myTFTWidth);
  void createDisplay();
  void createRectFrame(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t lineThickness, uint16_t colorBackgroung, uint16_t colorFrame);
  void createLoadingBar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t lineThickness, uint16_t colorBackgroung, uint16_t colorFrame, uint16_t colorBar, uint16_t barValue, bool showValue);
  // Set Bitmap Picture
  void drawBMPPicture(uint16_t x, uint16_t y, uint16_t bitMapWidth, uint16_t bitMapHeight, const char *path); // Check Parameters, Height, Width, Color, File

  // Set Display parameter
  void setGPS(signalGPS);
  void setObjectName(objectName);
  void setMessage(message);
  void setMode(mode);
  void setValue(text);
  void setValue(number);
};

#endif // VIGORTFT_H