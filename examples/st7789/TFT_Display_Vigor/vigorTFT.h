//
// Created by lukas on 05.03.2025.
//

#pragma once

#include <chrono>
#include <thread>
#include <iostream>
#include <cstring>
#include <hiredis/hiredis.h>
#include "ST7789_TFT_LCD_RVL.hpp"
#include "layout.h"

#define myTFTWidth 240
#define myTFTHeight 320
#define logoVigorWidth 240	
#define logoVigorHeight 122
#define pathLogoVigor "bitmap/Vigor_Logo_o_Hg_16-bitRGB565.bmp"
const std::string vigorVersion = "Vigor TFT version 1.0";

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
  void createInitDisplay();
  void createLoadingBar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t lineThickness, uint16_t colorFrame, uint16_t colorBar, uint16_t barValue, bool showValue);
  void createDisplay(const std::unordered_map<std::string, std::string> &data, const std::string &currentState);
  // Set Bitmap Picture
  void drawBMPPicture(uint16_t x, uint16_t y, uint16_t bitMapWidth, uint16_t bitMapHeight, const char *path); // Check Parameters, Height, Width, Color, File
  void drawText(const TextBoxInstance &textBox, const std::string &text = ""); // Draw text in TextBoxInstance, if text is empty, use defaultText from TextBoxDefinition
};
