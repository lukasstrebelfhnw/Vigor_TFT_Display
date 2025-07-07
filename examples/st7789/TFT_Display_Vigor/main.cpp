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
#include <iostream>
#include <chrono>
#include <thread>
#include <hiredis/hiredis.h>
#include "vigorTFT.h"
#include <vector>
#include <unordered_map>

#define cycleTimeMs 3000 // Zykluszeit in Millisekunden
// Picture ::  Defines
#define logoVigorWidth 240	// Set Width of Picture
#define logoVigorHeight 122 // Set Height of Picture
#define pathLogoVigor "bitmap/Vigor_Logo_o_Hg_16-bitRGB565.bmp"

// Temporary ::  Defines
const std::string vigorVersion = "Vigor TFT version 1.0";

// Display size in pixels
#define myTFTWidth 240
#define myTFTHeight 320
// Initialize TFT
vigorTFT myVigorTFT;
// Function to read from Redis
using RedisData = std::unordered_map<std::string, std::string>;

//  Section ::  Function Headers
RedisData readRedis();
uint8_t SetupHWSPI(void); // setup + user options for hardware SPI 0

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



int main()
{
	if (SetupHWSPI() != 0)
		return -1; // Hardware SPI 0 initialisieren fehlgeschlagen

	
	std::cout << "State: StartUp" << std::endl;
	myVigorTFT.createInitDisplay(logoVigorWidth, logoVigorHeight, pathLogoVigor, vigorVersion, myTFTWidth, myTFTHeight);

	while (true)
	{
		// read redis data
		RedisData data = readRedis();

		auto stateIt = data.find("hmi_state");
		if (stateIt == data.end())
		{
			std::cerr << "Fehler: hmi_state nicht in Redis-Daten gefunden!" << std::endl;
			currentState = HMIState::ERROR; // Fallback zu ERROR, falls kein State in Redis gefunden wird
		}
		else
		{
			const std::string &stateString = stateIt->second;
			myVigorTFT.createDisplay(data, stateString);
		}

		/*!!IMPOTANT!!!*/
		////////////////
		// for operation mode set sleep-for to 500ms
		////////////////
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
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

// *************** EOF ****************
