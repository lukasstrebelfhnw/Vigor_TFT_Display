#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include "ST7789_TFT_LCD_RVL.hpp"


struct TextBoxDefinition {
    std::string id;
    std::string defaultText;
    int x;
    int y;
    int width;
    int height;
};

struct TextBoxInstance {
    std::string textboxId;
    uint16_t color;
};

struct Screen {
    std::string name;
    std::vector<TextBoxInstance> elements;
};