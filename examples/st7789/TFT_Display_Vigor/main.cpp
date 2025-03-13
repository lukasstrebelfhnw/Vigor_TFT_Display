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
#include <vector>
#include <unordered_map>
#include "TextBox.h"

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
// #define pathLogoVigor "bitmap/bitmap16images/Vigor_Logo_o_Hg_16-bitRGB565.bmp"
#define pathLogoVigor "bitmap/bitmap16images/Vigor_Logo_Display.BMP"
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

std::unordered_map<std::string, TextBox> textBoxes = {
	{"hmi_vend_ist", {"INIT", 10, 96, 96, 48}},								  // INIT
	{"hmi_calibrated", {"INIT", 8, 10, 304, 32}},							  // INIT
	{"hmi_vend_ist_L", {"CALIB;SEMI;MANUAL_L;EDGE_L;AUTO", 42, 65, 32, 16}},  // CALIB/SEMI/MAN/L/EDGEL/AUTO
	{"hmi_vend_ist_R", {"CALIB;SEMI;MANUAL_R;EDGE_R;AUTO", 172, 65, 32, 16}}, // SEMI/MAN/L_R/EDGEL_R/AUTO
	{"hmi_vend_soll", {"CALIB", 10, 96, 128, 48}},							  // CALIB
	{"hmi_pos_l", {"SEMI;MANUAL_L;EDGE_L;AUTO", 10, 96, 128, 48}},			  // SEMI/MAN/L_R/EDGEL_R/AUTO
	{"hmi_pos_r", {"SEMI;MANUAL_R;EDGE_R;AUTO", 140, 96, 128, 48}},			  // SEMI/MAN/L_R/EDGEL_R/AUTO
	{"hmi_soll_l", {"EDGE_L;AUTO", 42, 159, 32, 16}},						  // EDGEL_R/AUTO
	{"hmi_soll_r", {"EDGE_R;AUTO", 172, 159, 32, 16}},						  // EDGEL_R/AUTO
	{"hmi_speed", {"EDGE_L;EDGE_R;AUTO", 10, 214, 88, 16}},					  // EDGEL_R/AUTO
	{"hmi_gps", {"EDGE_L;EDGE_R;AUTO", 148, 214, 88, 16}},					  // EDGEL_R/AUTO
	{"hmi_feldname", {"EDGE_L;EDGE_R;AUTO", 10, 10, 120, 16}},				  // EDGEL_R/AUTO max Feldname 15 Zeichen 16x16
	//{"hmi_state", {"",10, 130, 220, 20}},											 // only for Statemachine
	{"hmi_fehler", {"ERROR", 16, 16, 288, 32}},								   // ERROR code 18 Zeichen 16x32
	{"hmi_fehler_firstline", {"ERROR", 16, 52, 288, 16}},					   // ERROR firstline 36 Zeichen 8x16
	{"hmi_button1_3Z", {"MANUAL_L;MANUAL_R;EDGE_L;EDGE_R", 262, 14, 48, 32}},  // /MAN/L_R/EDGEL_R  "auf"
	{"hmi_button1_1Z", {"CALIB", 294, 14, 16, 32}},							   // CALIB "+"
	{"hmi_button2_2Z", {"MANUAL_L;MANUAL_R;EDGE_L;EDGE_R", 278, 74, 32, 32}},  // MAN/L_R/EDGEL_R "zu"
	{"hmi_button2_1Z", {"CALIB", 294, 74, 16, 32}},							   // CALIB "-"
	{"hmi_button3_2Z", {"INIT", 278, 134, 32, 32}},							   // INIT "JA"
	{"hmi_button4_3Z", {"MANUAL_L;MANUAL_R;EDGE_L;EDGE_R", 262, 194, 48, 32}}, // MAN/L_R/EDGEL_R "L/R"
	{"hmi_button4_4Z", {"INIT", 246, 194, 64, 32}},							   // INIT "NEIN"
	{"hmi_button4_8Z", {"CALIB", 182, 194, 64, 32}},						   // CALIB "abdrehen"
	{"hmi_button4_10Z", {"ERROR", 1150, 194, 64, 32}},						   // ERROR "quittieren"
};

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

	// only for testing

	//  Verfügbare States für hmi_state (du kannst hier gerne anpassen)
	std::vector<std::string> possibleStates = {
		"INIT", "CALIB", "SEMI", "MANUAL_L", "MANUAL_R", "EDGE_L", "EDGE_R", "AUTO", "ERROR"};

	// Zufalls-Seed setzen
	std::srand(std::time(nullptr));

	// Jeden Key mit einem zufälligen Wert füllen
	for (const auto &key : keys)
	{
		std::string value;

		if (key == "hmi_state")
		{
			// Einen zufälligen State auswählen
			value = possibleStates[std::rand() % possibleStates.size()];
		}
		else if (key == "hmi_feldname")
		{
			// Beispiel-Feldname mit bis zu 15 Zeichen
			int len = 5 + std::rand() % 10;
			value = "Feld_";
			for (int i = 0; i < len; ++i)
			{
				value += static_cast<char>('A' + std::rand() % 26);
			}
		}
		else if (key == "hmi_fehler")
		{
			value = "Fehlercode " + std::to_string(100 + std::rand() % 900);
		}
		else
		{
			// Zufallszahl als String (1–4 Stellen)
			int number = std::rand() % 10000; // 0–9999
			value = std::to_string(number);
		}

		// Wert in Redis schreiben
		redisReply *reply = (redisReply *)redisCommand(c, "SET %s %s", key.c_str(), value.c_str());
		if (reply)
			freeReplyObject(reply);
	}

	std::cout << "Zufallswerte in Redis geschrieben." << std::endl;

	// End of testing

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

// State Machine Execution
void runHMIStateMachine(HMIState state, const RedisData &data)
{
	// Den aktuellen State als String bekommen
	std::string currentStateString;
	switch (state)
	{
	case HMIState::INIT:
		currentStateString = "INIT";
		break;
	case HMIState::CALIB:
		currentStateString = "CALIB";
		break;
	case HMIState::SEMI:
		currentStateString = "SEMI";
		break;
	case HMIState::MANUAL_L:
		currentStateString = "MANUAL_L";
		break;
	case HMIState::MANUAL_R:
		currentStateString = "MANUAL_R";
		break;
	case HMIState::EDGE_L:
		currentStateString = "EDGE_L";
		break;
	case HMIState::EDGE_R:
		currentStateString = "EDGE_R";
		break;
	case HMIState::AUTO:
		currentStateString = "AUTO";
		break;
	case HMIState::ERROR:
		currentStateString = "ERROR";
		break;
	default:
		std::cerr << "Fehler: Ungültiger HMI-State!" << std::endl;
		return;
	}

	// TFT-Display basierend auf dem aktuellen State updaten
	myVigorTFT.createDisplay(data, textBoxes, currentStateString, myTFTHeight, myTFTWidth);
}

// State Machine Execution
void runHMIStateMachine(HMIState state)
{
	switch (state)
	{
	case HMIState::STARTUP:
		std::cout << "State: StartUp" << std::endl;
		myVigorTFT.createInitDisplay(logoVigorWidth, logoVigorHeight, pathLogoVigor, vigorVersion, myTFTWidth, myTFTHeight);
		break;
	default:
		std::cerr << "Fehler: runHMIStateMachine wurde ohne Redis-Daten mit einem falschen State aufgerufen!" << std::endl;
		break;
	}
}

int main()
{
	if (SetupHWSPI() != 0)
		return -1; // Hardware SPI 0 initialisieren fehlgeschlagen

	// 🏁 1️⃣ Starte mit dem Startup-State
	HMIState currentState = HMIState::STARTUP;
	runHMIStateMachine(currentState); // Leeres `data`, da Startup keine Daten braucht

	while (true)
	{
		// 🔄 2️⃣ Redis-Daten auslesen
		RedisData data = readRedis();

		// 📌 Versuchen, den aktuellen State aus Redis zu lesen
		auto stateIt = data.find("hmi_state");
		if (stateIt == data.end())
		{
			std::cerr << "Fehler: hmi_state nicht in Redis-Daten gefunden!" << std::endl;
			currentState = HMIState::ERROR; // Fallback zu ERROR, falls kein State in Redis gefunden wird
		}
		else
		{
			// 📝 State aus Redis holen
			const std::string &stateString = stateIt->second;

			// 🔄 String-State in `HMIState` umwandeln
			if (stateString == "INIT")
				currentState = HMIState::INIT;
			else if (stateString == "CALIB")
				currentState = HMIState::CALIB;
			else if (stateString == "SEMI")
				currentState = HMIState::SEMI;
			else if (stateString == "MANUAL_L")
				currentState = HMIState::MANUAL_L;
			else if (stateString == "MANUAL_R")
				currentState = HMIState::MANUAL_R;
			else if (stateString == "EDGE_L")
				currentState = HMIState::EDGE_L;
			else if (stateString == "EDGE_R")
				currentState = HMIState::EDGE_R;
			else if (stateString == "AUTO")
				currentState = HMIState::AUTO;
			else if (stateString == "ERROR")
				currentState = HMIState::ERROR;
			else
				currentState = HMIState::STARTUP; // Fallback-Wert, falls ungültig
		}

		// 🏁 3️⃣ Neuen Zustand mit Daten aus Redis ausführen
		runHMIStateMachine(currentState, data);

		// 🕒 Wartezeit zwischen den State-Updates
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

void EndTests(void)
{
	myVigorTFT.fillScreen(RVLC_BLACK);
	myVigorTFT.TFTPowerDown(); // Power down device
	bcm2835_close();		   // Close the bcm2835 library
	std::cout << "TFT End" << std::endl;
}

// *************** EOF ****************
