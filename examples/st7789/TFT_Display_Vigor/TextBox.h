#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <string>

struct TextBox
{
    std::string useableSTATES;
    int16_t x;
    int16_t y;
    int16_t width;
    int16_t height;
    int16_t color;
};

#endif // TEXTBOX_H