/*! 
	@file examples/st7789/Hello_world/main.cpp
	@brief Hello World software SPI test
	@author Gavin Lyons.
	@note See USER OPTIONS 1-3 in SETUP function, run as sudo
		Tests  software SPI 
	@test 
		-# 102 Hello World Software SPI
*/

// Section ::  libraries 
#include <bcm2835.h> 
#include "ST7789_TFT_LCD_RVL.hpp"

// Section :: Globals 
ST7789_TFT myTFT;
// Display size in pixels
#define myTFTWidth 240
#define myTFTHeight 320

//  Section ::  Function Headers 

uint8_t SetupSWSPI(void); // setup + user options for software SPI
void HelloWorld(void);
void EndTests(void);

//  Section ::  MAIN loop

int main() 
{
	if(SetupSWSPI() != 0) return -1; //Software SPI
	HelloWorld();
	EndTests();
	return 0;
}
// *** End OF MAIN **


//  Section ::  Function Space 

// Software SPI setup
uint8_t SetupSWSPI(void)
{
	std::cout << "TFT Start Test 102 SW SPI" << std::endl;
	if(!bcm2835_init())
	{
		std::cout << "Error 1201 Problem with init bcm2835 library" << std::endl;
		return 2;
	}

// ** USER OPTION 1 GPIO/SPI TYPE SW , pick any u want**
	int8_t RST_TFT  = 25;
	int8_t DC_TFT   = 24;
	int8_t SCLK_TFT = 26;
	int8_t SDIN_TFT = 19;
	int8_t CS_TFT   = 12;
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
//*********************************************

// ** USER OPTION 2 Screen Setup **
	uint8_t OFFSET_COL = 0;  // These offsets can be adjusted for any issues->
	uint8_t OFFSET_ROW = 0;  // with manufacture tolerance/defects
	uint16_t TFT_WIDTH = myTFTWidth;    // Screen width in pixels
	uint16_t TFT_HEIGHT = myTFTHeight;  // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
// ***********************************

// ** USER OPTION 3 SPI **
	uint16_t SWSPI_CommDelay = 0; //uS GPIO SW SPI delay
	myTFT.TFTInitSPI(SWSPI_CommDelay);
//*****************************
	std::cout << "ST7789 library version : " << GetRVLibVersionNum()<< std::endl;
	std::cout <<"bcm2835 library version :" << bcm2835_version() << std::endl;
	std::cout << "SWSPI Comm GPIO Delay set to : " << myTFT.HighFreqDelayGet()<< "uS" << std::endl;
	delayMilliSecRVL(100);
	return 0;
}

void HelloWorld(void) 
{
	std::cout << "Hello World" << std::endl;
	myTFT.TFTsetRotation(myTFT.TFT_Degrees_90); // Rotate the display
	myTFT.fillScreen(RVLC_BLACK);
	myTFT.setCursor(42,60);
	myTFT.setTextColor(RVLC_GREEN, RVLC_BLACK);
	myTFT.setFont(font_orla);
	myTFT.print("Hello world");
	delayMilliSecRVL(7000);
}

void EndTests(void)
{
	myTFT.fillScreen(RVLC_BLACK);
	myTFT.TFTPowerDown(); // Power down device
	bcm2835_close(); // Close the bcm2835 library
	std::cout << "TFT End" << std::endl;
}

// *************** EOF ****************
