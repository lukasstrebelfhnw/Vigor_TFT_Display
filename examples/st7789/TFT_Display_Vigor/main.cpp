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
#include <unordered_map>
#include <string>
#include <hiredis/hiredis.h>
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
#define pathLogoVigor "bitmap/bitmap16images/Vigor_Logo_o_Hg_16-bitRGB565.bmp"
// Temporary ::  Defines
const std::string vigorVersion = "Vigor TFT version 1.0";

vigorTFT myVigorTFT;

// Display size in pixels
#define myTFTWidth 240
#define myTFTHeight 320

//  Section ::  Function Headers

uint8_t SetupHWSPI(void); // setup + user options for hardware SPI 0
void EndTests(void);

// HMI States
enum class HMIState
{
	STARTUP,
	INIT,
	CALIB,
	MANUAL_L,
	MANUAL_R,
	SEMI,
	AUTO,
	EDGE_L,
	EDGE_R,
	ERROR
};

// Function to read from Redis
typedef std::unordered_map<std::string, std::string> RedisData;
RedisData readRedis()
{
	redisContext *c = redisConnect("127.0.0.1", 6379);
	RedisData data;
	if (c == nullptr || c->err)
	{
		std::cerr << "Redis Connection Error" << std::endl;
		return data;
	}
	std::vector<std::string> keys = {
		"hmi_vend_ist", "hmi_vend_soll", "hmi_pos_l", "hmi_pos_r",
		"hmi_soll_l", "hmi_soll_r", "hmi_feldname", "hmi_speed",
		"hmi_gps", "hmi_state", "hmi_fehler"};
	for (const auto &key : keys)
	{
		redisReply *reply = (redisReply *)redisCommand(c, "GET %s", key.c_str());
		if (reply && reply->type == REDIS_REPLY_STRING)
		{
			data[key] = reply->str;
		}
		freeReplyObject(reply);
	}
	redisFree(c);
	return data;
}

/*
// Function to update display
void updateDisplay(const RedisData& data) {
	myVigorTFT.fillScreen(ST77XX_BLACK);
	int y = 10;
	for (const auto& [key, value] : data) {
		myTFT.drawText(10, y, key + ": " + value, ST77XX_WHITE, ST77XX_BLACK);
		y += 20;
	}
}
*/

// State Machine Execution
void runHMIStateMachine(HMIState state)
{
	RedisData data = readRedis();
	switch (state)
	{
	case HMIState::STARTUP:
		std::cout << "State: StartUp" << std::endl;
		vigorTFT.createInitDisplay(logoVigorWidth, logoVigorHeight, pathLogoVigor, vigorVersion, myTFTHeight, myTFTWidth);
		break;
	case HMIState::INIT:
		std::cout << "State: Init" << std::endl;
		break;
	case HMIState::CALIB:
		std::cout << "State: Calib" << std::endl;
		break;
	case HMIState::MANUAL_L:
		std::cout << "State: Manual_L" << std::endl;
		break;
	case HMIState::MANUAL_R:
		std::cout << "State: Manual_R" << std::endl;
		break;
	case HMIState::SEMI:
		std::cout << "State: Semi" << std::endl;
		break;
	case HMIState::AUTO:
		std::cout << "State: Auto" << std::endl;
		break;
	case HMIState::EDGE_L:
		std::cout << "State: Edge_L" << std::endl;
		break;
	case HMIState::EDGE_R:
		std::cout << "State: Edge_R" << std::endl;
		break;
	case HMIState::ERROR:
		std::cout << "State: Error" << std::endl;
		break;
	}
	// updateDisplay(data);
}

int main()
{
	if (SetupHWSPI() != 0)
		return -1; // Hardware SPI 0

	HMIState currentState = HMIState::STARTUP;
	while (true)
	{
		runHMIStateMachine(currentState);
		std::this_thread::sleep_for(std::chrono::seconds(1));
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
