//
// Created by lukas on 05.03.2025.
//

#include "vigorTFT.h"

std::string vigorTFT::old_state = ""; // Initialize the static variable

vigorTFT::vigorTFT()
{
	// myVigorTFT = new vigorTFT(); // dinamic memory allocation is this allowed by Raspberry Pi? ask Dani
}

vigorTFT::~vigorTFT()
{
	// delete myVigorTFT;
}

void vigorTFT::createInitDisplay()
{
	/* If you cange the font, you have to change also the Hight and Width of the font
	Font definitions*/
	uint8_t versionFontHight = 16; // font_retro 16
	// End Font definitions

	// Set Display parameter

	uint16_t x = 40;			   // Set x Poition Logo effective Value left top corner Display
	uint16_t y = 15;			   // Set y Poition Logo effective Value left top corner Display
	uint16_t loadingBarHight = 38; // Important if is this value bigger than (fontHigh+1+2*lineThickness)
	uint16_t loadingBarWidth = myTFTWidth - (2 * x);
	uint16_t spaceMean = ((myTFTHeight - y - logoVigorHeight - loadingBarHight - versionFontHight) / 3);

	// Buils Display
	this->TFTsetRotation(this->TFT_Degrees_270); // Rotate the display
	this->fillScreen(RVLC_BLACK);
	this->drawBMPPicture(x, y, logoVigorWidth, logoVigorHeight, pathLogoVigor);

	this->setCursor(x * 2, y + logoVigorHeight + 2 * spaceMean + loadingBarHight); // set Cursor left top corner
	this->setFont(font_retro);													// select font
	this->setTextColor(RVLC_YELLOW, RVLC_BLACK);							// select color
	this->print(vigorVersion);

	for (int i = 0; i < 100; i++) // for-loop for loading bar
	{
		this->createLoadingBar(x, (y + logoVigorHeight + spaceMean), loadingBarWidth, loadingBarHight, 6, RVLC_GREEN, RVLC_DGREEN, i, true);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	this->fillScreen(RVLC_BLACK);
}

void vigorTFT::createDisplay(
	const std::unordered_map<std::string, std::string> &data,
	const std::string &currentState)
{
	// get global variable screens from layout
	extern std::unordered_map<std::string, Screen> screens;
	extern std::unordered_map<std::string, TextBoxDefinition> textBoxDefs;

	// get textboxes from screens for current state
	auto stateIt = screens.find(currentState);
	if (stateIt == screens.end())
	{
		std::cout << "Error: Current state not found in screens" << std::endl;
		return;
	}
	const auto &tb_instances = stateIt->second;

	if (old_state != currentState)
	{
		this->TFTsetRotation(this->TFT_Degrees_270); // Rotate the display
		this->fillScreen(RVLC_BLACK);
		old_state = currentState;

		for (const auto &tb_instance : tb_instances.elements)
		{ 
			// get textbox definition from textBoxDefs
			auto textBoxDefIt = textBoxDefs.find(tb_instance.textboxId);
			if (textBoxDefIt == textBoxDefs.end())
			{
				std::cout << "Error: Textbox definition not found for ID: " << tb_instance.textboxId << std::endl;
				continue; // Skip to the next textbox if the definition is not found
			}
			const TextBoxDefinition &textBoxDef = textBoxDefIt->second;

			// if not updateable then draw and else skip
			if (!textBoxDef.updateable)
			{
				auto dataIt = data.find(textBoxDef.id);
				if (dataIt != data.end())
				{
					// If the data is found, draw the text with the provided value
					this->drawText(tb_instance, dataIt->second);
				}
				else
				{
					// If the data is not found, draw the text with the default value
					this->drawText(tb_instance);
				}
			}
		}
	}

	
	for (const auto &tb_instance : tb_instances.elements)
	{ 
		// get textbox definition from textBoxDefs
		auto textBoxDefIt = textBoxDefs.find(tb_instance.textboxId);
		if (textBoxDefIt == textBoxDefs.end())
		{
			std::cout << "Error: Textbox definition not found for ID: " << tb_instance.textboxId << std::endl;
			continue; // Skip to the next textbox if the definition is not found
		}
		const TextBoxDefinition &textBoxDef = textBoxDefIt->second;

		// if updateable then draw and else skip
		if (textBoxDef.updateable)
		{
			auto dataIt = data.find(textBoxDef.id);
			if (dataIt != data.end())
			{
				// If the data is found, draw the text with the provided value
				this->drawText(tb_instance, dataIt->second);
			}
			else
			{
				// If the data is not found, draw the text with the default value
				this->drawText(tb_instance);
			}
		}
	}
}

void vigorTFT::createLoadingBar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t lineThickness, uint16_t colorFrame, uint16_t colorBar, uint16_t barValue, bool showValue)
{
	display_Font_name_e font = font_orla;
	uint16_t effectiveFontSizeWidth = 16;					   // this->getFontSizeWidth(&font);
	uint16_t effectiveFontSizeHeight = 24;					   // this->getFontSizeHeight(&font);
	uint16_t effectiveBarHeight = effectiveFontSizeHeight + 1; // this->getFontSizeHeight(&font);
	uint16_t effectiveBarWidth = w - 2 * lineThickness;
	this->fillRectangle(x, y, w, h, colorFrame);

	if (showValue)
	{
		uint16_t minBarWidth = 4 * effectiveFontSizeWidth + 2 * lineThickness + 10;
		effectiveBarWidth = w - 4 * effectiveFontSizeWidth - 3 * lineThickness; // this->getFontSizeWidth(&font);
		if ((effectiveBarHeight + 2 * lineThickness) >= h)
		{
			std::cout << "Error: Bar size height is too small" << std::endl;
			return;
		}
		if (w < minBarWidth)
		{
			std::cout << "Error: Bar size width is too small" << std::endl;
			return;
		}
		else
		{

			uint16_t filledWidth = (barValue * effectiveBarWidth) / 100;
			// this->fillRectangle(x + lineThickness, y + lineThickness, effectiveBarWidth, effectiveBarHeight, colorBackground); // Override the inner rectangle with the background color
			this->fillRectangle(x + lineThickness, y + lineThickness, filledWidth, effectiveBarHeight, colorBar); // Print progress bar
			this->setFont(font);																				  // select font
			this->setTextColor(colorBar, colorFrame);
			this->setCursor(x + 2 * lineThickness + effectiveBarWidth, y + lineThickness);
			this->print(std::to_string(barValue) + "%");
		}
	}

	else
	{
		if ((effectiveBarHeight + 2 * lineThickness) > h)
		{
			std::cout << "Error: Bar size height is too small" << std::endl;
			return;
		}
		if (w < (10 + 2 * lineThickness))
		{
			std::cout << "Error: Bar size width is too small" << std::endl;
			return;
		}
		else
		{
			uint16_t filledWidth = (barValue * effectiveBarWidth) / 100;
			// this->fillRectangle(x + lineThickness, y + lineThickness, effectiveBarWidth, effectiveBarHeight, colorBackground); // Override the inner rectangle with the background color
			this->fillRectangle(x + lineThickness, y + lineThickness, filledWidth, effectiveBarHeight, colorBar); // Print progress bar
		}
	}
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
			*pixel = RVLC_BLACK; // Ersetze durch die gewählte Farbe
		}
	}

	if (this->drawBitmap16(x, y, bmpBuffer, bitMapWidth, bitMapHeight) != rvlDisplay_Success)
	{
		std::cout << "Warning: An error occurred in drawBitmap16" << std::endl;
	}

	free(bmpBuffer);
}

void vigorTFT::drawText(const TextBoxInstance &textBox, const std::string &text)
{
	extern std::unordered_map<std::string, TextBoxDefinition> textBoxDefs; // Get the global variable textBoxDefs
	auto it = textBoxDefs.find(textBox.textboxId);
	if (it == textBoxDefs.end())
	{
		std::cout << "Error: TextBoxDefinition not found for ID: " << textBox.textboxId << std::endl;
		return;
	}
	const TextBoxDefinition &textBoxDef = it->second;
	this->setCursor(textBoxDef.x, textBoxDef.y);
	if (textBoxDef.height == 16)
	{
		this->setFont(font_retro);
	}
	else if (textBoxDef.height == 32)
	{
		this->setFont(font_groTesk);
	}
	else if (textBoxDef.height == 48)
	{
		this->setFont(font_mint);
	}
	else
	{
		std::cout << "Error: Unsupported text box height: " << textBoxDef.height << std::endl;
		return;
	}
	this->setTextColor(textBox.color, RVLC_BLACK); // Set text color and background
	if (text.empty())
	{
		this->print(textBoxDef.defaultText); // Print the default text
	}
	else
	{
		this->print(text); // Print the provided text
	}
}
	
