//
// Created by lukas on 05.03.2025.
//

#include "vigorTFT.h"
#include <ctime>
#include <chrono>
#include <thread>
#include <string>
#include <iostream>
#include <cstring>
#include "ST7789_TFT_LCD_RVL.hpp"

// Color definitions 16-Bit Color Values R5G6B5 from Vigor Frontpaneel
#define buttonGrey 0x52EC
#define buttonAuto 0x1389
#define buttonSemi 0xB666
#define buttonMan 0xFE88
#define buttonRand 0x543A
#define backGroundColor 0x9D14
#define vigorDGreen 0x73E6
#define vigorLGreen 0xADE6

vigorTFT::vigorTFT()
{
	// myVigorTFT = new vigorTFT(); // dinamic memory allocation is this allowed by Raspberry Pi? ask Dani
}

vigorTFT::~vigorTFT()
{
	// delete myVigorTFT;
}

void vigorTFT::createInitDisplay(uint16_t bitMapWidth, uint16_t bitMapHeight, const char *path, std::string versionVigor, uint16_t myTFTHeight, uint16_t myTFTWidth)
{
	/* If you cange the font, you have to change also the Hight and Width of the font
	Font definitions*/
	uint8_t versionFontHight = 16; // font_retro 16
	uint8_t versionFontWidth = 8;  // font_retro 8
	// End Font definitions

	// Set Display parameter

	uint16_t x = 40; // Set x Poition Logo effective Value left top corner Display
	uint16_t y = 15; // Set y Poition Logo effective Value left top corner Display
	uint16_t loadingBarHight = 2 * versionFontHight;
	uint16_t loadingBarWidth = myTFTWidth - (4 * x);
	uint16_t spaceMean = ((myTFTHeight - y - bitMapHeight - loadingBarHight - versionFontHight) / 3);
	uint16_t versionText_x = (myTFTWidth - (versionVigor.length() * versionFontWidth)) / 2; // Set x Poition versionText effective Value left top corner Display
	uint16_t versionText_y = y + bitMapHeight + 2 * spaceMean + loadingBarHight;			// Set y Poition versionText effective Value left top corner Display

	// Buils Display
	this->TFTsetRotation(this->TFT_Degrees_90); // Rotate the display
	this->fillScreen(backGroundColor);
	this->drawBMPPicture(x, y, bitMapWidth, bitMapHeight, path);

	this->setCursor(x * 2, y + bitMapHeight + 2 * spaceMean + loadingBarHight); // set Cursor left top corner
	this->setFont(font_retro);													// select font
	this->setTextColor(buttonRand, backGroundColor);							// select color
	this->print(versionVigor);

	for (int i = 0; i < 100; i++) // for-loop for loading bar
	{
		this->createLoadingBar((x * 2), (y + bitMapHeight + spaceMean), loadingBarWidth, loadingBarHight, 6, backGroundColor, buttonAuto, buttonSemi, i, true);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	this->fillScreen(RVLC_BLACK);
}

void vigorTFT::createDisplay()
{
	/*
	std::cout << "Init" << std::endl;
	myTFT.TFTsetRotation(myTFT.TFT_Degrees_90); // Rotate the display
	myTFT.fillScreen(backGroundColor);
	myTFT.setCursor(42, 60);
	myTFT.fillRect(0, 0, 320, 10, RVLC_GREEN);
	myTFT.fillRect(0, 20, 320, 10, RVLC_DGREEN);
	myTFT.setTextColor(RVLC_WHITE, RVLC_BLACK);
	myTFT.setFont(font_groTesk);
	myTFT.setTextColor(buttonAuto, buttonRand); // first text last background
	myTFT.setFont(font_orla);
	myTFT.print("Hello Kathrin");
	delayMilliSecRVL(7000);
	*/
}

void vigorTFT::createRectFrame(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t lineThickness, uint16_t colorBackgroung, uint16_t colorFrame)
{
	this->fillRectangle(x, y, w, w, colorFrame);
	this->fillRectangle(x + lineThickness, y + lineThickness, w - 2 * lineThickness, h - 2 * lineThickness, colorBackgroung);
}

void vigorTFT::createLoadingBar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t lineThickness, uint16_t colorBackground, uint16_t colorFrame, uint16_t colorBar, uint16_t barValue, bool showValue)
{
	display_Font_name_e font = font_orla;
	uint16_t effectiveBarHeight = this->getFontSizeHeight(&font);
	uint16_t effectiveBarWidth = w - 2 * lineThickness;
	this->fillRectangle(x, y, w, h, colorFrame);

	// Override the inner rectangle with the background color
	// this->fillRectangle(x + lineThickness, y + lineThickness, w - 2 * lineThickness, h - 2 * lineThickness, colorBackground);

	if (showValue)
	{
		effectiveBarWidth = w - 4 * this->getFontSizeWidth(&font);
		if ((effectiveBarHeight - 2) >= h)
		{
			std::cout << "Error: Bar size height is too small" << std::endl;
			return;
		}
		if (2 * effectiveBarWidth > w)
		{
			std::cout << "Error: Bar size width is too small" << std::endl;
			return;
		}
		else
		{

			uint16_t filledWidth = (barValue * effectiveBarWidth) / 100;
			lineThickness = (w - effectiveBarHeight) / 2;
			this->fillRectangle(x + lineThickness, y + lineThickness, effectiveBarWidth - 2 * lineThickness, effectiveBarHeight - 2 * lineThickness, colorBackground); // Override the inner rectangle with the background color
			this->fillRectangle(x + lineThickness, y + lineThickness, filledWidth, effectiveBarHeight - 2 * lineThickness, colorBar);								   // Print progress bar
			this->setFont(font);																																	   // select font
			this->setTextColor(colorBar, colorFrame);
			this->setCursor(x + lineThickness, y + lineThickness);
			this->print(std::to_string(barValue) + "%");
		}
		else
		{
			uint16_t filledWidth = (barValue * (w - 2 * lineThickness)) / 100;
			this->fillRectangle(x + lineThickness, y + lineThickness, effectiveBarWidth - 2 * lineThickness, effectiveBarHeight - 2 * lineThickness, colorBackground); // Override the inner rectangle with the background color
			this->fillRectangle(x + lineThickness, y + lineThickness, filledWidth, effectiveBarHeight - 2 * lineThickness, colorBar);								   // Print progress bar
		}
	}
}

void vigorTFT::createTextBox(int16_t x, int16_t y, display_Font_name_e font, uint16_t textColor, std::string text)
{
	this->setCursor(x, y);
	this->setFont(font);		   // select font
	this->setTextColor(textColor); // first text last background
	this->print(text);
}

void vigorTFT::createTextBox(int16_t x, int16_t y, display_Font_name_e font, uint16_t textColor, uint16_t toggleTextColor, std::string text, bool toggleColor)
{
	this->setCursor(x, y);
	this->setFont(font); // select font
	if (toggleColor)
	{
		this->setTextColor(toggleTextColor); // first text last background
	}
	else
	{
		this->setTextColor(textColor); // first text last background
	}
	this->print(text);
}

void vigorTFT::drawBMPPicture(uint16_t x, uint16_t y, uint16_t bitMapWidth, uint16_t bitMapHeight, const char *path)
{
	std::cout << "BMPPicture: 16 bit color image bitmaps from file " << path << std::endl;
	// this->fillScreen(RVLC_BLACK);

	FILE *pFile = fopen(path, "rb");
	if (pFile == nullptr)
	{
		std::cout << "Error: File does not exist -> " << path << std::endl;
		return;
	}

	// Lese Bilddaten-Offset aus BMP-Header
	fseek(pFile, 10, SEEK_SET);
	uint32_t dataOffset = 0;
	fread(&dataOffset, sizeof(dataOffset), 1, pFile);
	fseek(pFile, dataOffset, SEEK_SET);

	size_t pixelCount = bitMapWidth * bitMapHeight;
	size_t bufferSize = pixelCount * 2;

	uint8_t *bmpBuffer = (uint8_t *)malloc(bufferSize);
	if (bmpBuffer == nullptr)
	{
		std::cout << "Error: Could not allocate memory" << std::endl;
		fclose(pFile);
		return;
	}

	size_t readBytes = fread(bmpBuffer, 1, bufferSize, pFile);
	fclose(pFile);

	if (readBytes != bufferSize)
	{
		std::cout << "Error: Unexpected file size (" << readBytes << " bytes read, expected " << bufferSize << ")" << std::endl;
		free(bmpBuffer);
		return;
	}

	// **Weiß ersetzen (RGB565: 0xFFFF)**
	for (size_t i = 0; i < pixelCount; i++)
	{
		uint16_t *pixel = (uint16_t *)&bmpBuffer[i * 2];
		if (*pixel == 0xFFFF) // Wenn der Pixel weiß ist
		{
			*pixel = backGroundColor; // Ersetze durch die gewählte Farbe
		}
	}

	if (this->drawBitmap16(x, y, bmpBuffer, bitMapWidth, bitMapHeight) != rvlDisplay_Success)
	{
		std::cout << "Warning: An error occurred in drawBitmap16" << std::endl;
	}

	free(bmpBuffer);
}

void vigorTFT::setGPS(signalGPS)
{
}

void vigorTFT::setObjectName(objectName)
{
}

void vigorTFT::setMessage(message)
{
}

void vigorTFT::setMode(mode)
{
}

void vigorTFT::setValue(text)
{
}

void vigorTFT::setValue(number)
{
}