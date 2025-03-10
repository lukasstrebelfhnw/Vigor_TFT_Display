//
// Created by lukas on 05.03.2025.
//

#ifndef VIGORTFT_H
#define VIGORTFT_H

// Color definitions 16-Bit Color Values R5G6B5 from Vigor Frontpaneel
#define buttonGrey 0x52EC
#define buttonAuto 0x1389
#define buttonSemi 0xB666
#define buttonMan 0xFE88
#define buttonRand 0x543A
#define backGroundColor 0x9D14
#define vigorDGreen 0x73E6
#define vigorLGreen 0xADE6
#include "ST7789_TFT_LCD_RVL.hpp"
#include <string>

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
  vigorTFT();
  ~vigorTFT() {};

  // Creates Display with text box
  void createDisplay();

  // Set Display parameter
  void setGPS(signalGPS);
  void setObjectName(objectName);
  void setMessage(message);
  void setMode(mode);
  void setValue(text);
  void setValue(number);
};

#endif // VIGORTFT_H