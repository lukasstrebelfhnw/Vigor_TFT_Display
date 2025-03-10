/*!
	@file examples/st7789/Hello_world/main.cpp
	@brief Hello World hardware SPI 0 test
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function, run as sudo
		Tests hardware SPI 0
	@test
		-# 101 Hello World Hardware SPI
*/

// Section ::  libraries
#include <bcm2835.h>
#include "ST7789_TFT_LCD_RVL.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include "vigorTFT.h"

// Color ::  Defines
#define buttonGrey 0x52EC
#define buttonAuto 0x1389
#define buttonSemi 0xB666
#define buttonMan 0xFE88
#define buttonRand 0x543A
#define backgroundColor 0x9D14
#define vigorDGreen 0x73E6
#define vigorLGreen 0xADE6
// Delay :: Defines
#define cycleTimeMs 3000 // Zykluszeit in Millisekunden
// Picture ::  Defines
#define logoVigorWidth 240	// Set Width of Picture
#define logoVigorHeight 122 // Set Height of Picture
#define pathLogoVigor "bitmap/bitmap16images/Vigor_Logo_o_Hg.bmp";
// Temporary ::  Defines
#define vigorVersion "Vigor TFT version 1.0"

vigorTFT myVigorTFT;

// Display size in pixels
#define myTFTWidth 240
#define myTFTHeight 320

//  Section ::  Function Headers

uint8_t SetupHWSPI(void); // setup + user options for hardware SPI 0
void EndTests(void);

//  Section ::  MAIN loop
enum State
{
	Init,
	Kalibrieren,
	Auto,
	Semi,
	Manuell,
	Randstreuen,
	Error
};

int main()
{
	// Initialisierung
	bool finishedInit = false; // Flag für Initialisierung
	State currentState = Init;

	if (SetupHWSPI() != 0)
		return -1; // Hardware SPI 0

	// Endlosschleife für die State-Machine
	while (true)
	{
		// Verarbeite aktuellen Zustand
		switch (currentState)
		{
		case Init:
			std::cout << "INIT" << std::endl;
			bool finishedInit = myVigorTFT.createInitDisplay(logoVigorWidth, logoVigorHeight, pathLogoVigor, vigorVersion);

			break;
		case Kalibrieren:
			std::cout << "Kalibrieren State\n";
			myVigorTFT.TFTsetRotation(myVigorTFT.TFT_Degrees_90); // Rotate the display
			myVigorTFT.fillScreen(backgroundColor);
			myVigorTFT.setCursor(5, 120);
			myVigorTFT.fillRect(0, 0, 320, 10, RVLC_GREEN);
			myVigorTFT.fillRect(0, 20, 320, 10, RVLC_DGREEN);
			myVigorTFT.setTextColor(buttonAuto, buttonRand); // first text last background
			myVigorTFT.setFont(font_arialRound);
			myVigorTFT.print("hesch au schochli hunger?");
			break;
		case Auto:
			std::cout << "Auto State\n";
			myVigorTFT.TFTsetRotation(myVigorTFT.TFT_Degrees_90); // Rotate the display
			myVigorTFT.fillScreen(backgroundColor);
			myVigorTFT.setCursor(5, 120);
			myVigorTFT.fillRect(0, 0, 320, 10, RVLC_GREEN);
			myVigorTFT.fillRect(0, 20, 320, 10, RVLC_DGREEN);
			myVigorTFT.setTextColor(buttonAuto, vigorLGreen); // first text last background
			myVigorTFT.setFont(font_arialBold);
			myVigorTFT.print("ich han saumässig hunger, gange aber glaub jetzt no go jogge");
			break;
		case Semi:
			std::cout << "Semi State\n";
			myVigorTFT.TFTsetRotation(myVigorTFT.TFT_Degrees_90); // Rotate the display
			myVigorTFT.fillScreen(backgroundColor);
			myVigorTFT.setCursor(160, 120);
			myVigorTFT.fillRect(0, 0, 320, 10, RVLC_GREEN);
			myVigorTFT.fillRect(0, 20, 320, 10, RVLC_DGREEN);
			myVigorTFT.setTextColor(buttonAuto, buttonSemi); // first text last background
			myVigorTFT.setFont(font_orla);
			myVigorTFT.print("Semi");
			break;
		case Manuell:
			std::cout << "Manuell State\n";
			myVigorTFT.TFTsetRotation(myVigorTFT.TFT_Degrees_90); // Rotate the display
			myVigorTFT.fillScreen(backgroundColor);
			myVigorTFT.setCursor(160, 120);
			myVigorTFT.fillRect(0, 0, 320, 10, RVLC_GREEN);
			myVigorTFT.fillRect(0, 20, 320, 10, RVLC_DGREEN);
			myVigorTFT.setTextColor(buttonAuto, buttonMan); // first text last background
			myVigorTFT.setFont(font_orla);
			myVigorTFT.print("Manuell");
			break;
		case Randstreuen:
			std::cout << "Randstreuen State\n";
			myVigorTFT.TFTsetRotation(myVigorTFT.TFT_Degrees_90); // Rotate the display
			myVigorTFT.fillScreen(backgroundColor);
			myVigorTFT.setCursor(160, 120);
			myVigorTFT.fillRect(0, 0, 320, 10, RVLC_GREEN);
			myVigorTFT.fillRect(0, 20, 320, 10, RVLC_DGREEN);
			myVigorTFT.setTextColor(buttonAuto, buttonRand); // first text last background
			myVigorTFT.setFont(font_orla);
			myVigorTFT.print("Randstreuen");
			break;
		case Error:
			std::cout << "Error State\n";
			myVigorTFT.TFTsetRotation(myVigorTFT.TFT_Degrees_90); // Rotate the display
			myVigorTFT.fillScreen(backgroundColor);
			myVigorTFT.setCursor(160, 120);
			myVigorTFT.fillRect(0, 0, 320, 10, RVLC_GREEN);
			myVigorTFT.fillRect(0, 20, 320, 10, RVLC_DGREEN);
			myVigorTFT.setTextColor(buttonAuto, vigorDGreen); // first text last background
			myVigorTFT.setFont(font_orla);
			myVigorTFT.print("Init");
			break;
		default:
			std::cout << "Unknown State\n";
			break;
		}

		// Nächsten Zustand bestimmen only for testing
		if (currentState == Init)
		{
			if (finishedInit)
			{
				currentState = Kalibrieren;
			}
		}
		else if (currentState == Kalibrieren)
		{
			currentState = Auto;
		}
		else if (currentState == Auto)
		{
			currentState = Init;
		}
		else if (currentState == Semi)
		{
			currentState = Manuell;
		}
		else if (currentState == Manuell)
		{
			currentState = Randstreuen;
		}
		else if (currentState == Randstreuen)
		{
			currentState = Error;
		}
		else
		{
			currentState = Init; // Fehlerbehandlung und Neustart
		}

		// Warten gemäß Zykluszeit
		std::this_thread::sleep_for(std::chrono::milliseconds(cycleTimeMs));
	}
	return 0;
}
// *** End OF MAIN **

//  Section ::  Function Space

// Hardware SPI setup
uint8_t SetupHWSPI(void)
{
	std::cout << "TFT_Display_Vigor" << std::endl;
	if (!bcm2835_init())
	{
		std::cout << "Error 1201 Problem with init bcm2835 library" << std::endl;
		return 2;
	}

	// ** USER OPTION 1 GPIO HW SPI **
	int8_t RST_TFT = 25;
	int8_t DC_TFT = 24;
	// myTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
	myVigorTFT.TFTSetupGPIO(RST_TFT, DC_TFT);
	//*********************************************

	// ** USER OPTION 2 Screen SetupHWSPI **
	uint16_t OFFSET_COL = 0;		   // These offsets can be adjusted for any issues->
	uint16_t OFFSET_ROW = 0;		   // with manufacture tolerance/defects at edge of display
	uint16_t TFT_WIDTH = myTFTWidth;   // Screen width in pixels
	uint16_t TFT_HEIGHT = myTFTHeight; // Screen height in pixels
	// myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
	myVigorTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
	// ***********************************

	// ** USER OPTION 3 SPI baud rate + SPI_CE_PIN**
	uint32_t SCLK_FREQ = 8000000;											//  freq in Hertz , MAX 125 Mhz MIN 30Khz
	uint8_t SPI_CE_PIN = 0;													// which HW SPI chip enable pin to use,  0 or 1
	if (myVigorTFT.TFTInitSPI(SCLK_FREQ, SPI_CE_PIN) != rvlDisplay_Success) // If there is a problem, undo myVigorTFT to myTFT
	{
		bcm2835_close(); // Close lib & /dev/mem, deallocating mem
		return 3;
	}
	//*****************************
	std::cout << "ST7789 library version : " << GetRVLibVersionNum() << std::endl;
	std::cout << "bcm2835 library version :" << bcm2835_version() << std::endl;
	delayMilliSecRVL(100);
	return 0;
}

void EndTests(void)
{
	myVigorTFT.fillScreen(RVLC_BLACK);
	myVigorTFT.TFTPowerDown(); // Power down device
	bcm2835_close();		   // Close the bcm2835 library
	std::cout << "TFT End" << std::endl;
}

// *************** EOF ****************
