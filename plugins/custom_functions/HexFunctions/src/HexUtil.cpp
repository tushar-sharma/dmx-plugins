/*******************************************************************************

 Copyright (c) 2017-present

 *******************************************************************************/
#include <string>
#include <cctype>
#include "HexUtil.h"

/******************************************************************************/

std::string HexUtil::hexToText(const std::string &hexValue) {

    std::string text  = std::string((hexValue.size() + 1) >> 1, ' ');
    size_t final_text_length = hexValue.size() / 2;

    for (size_t i = 0, j = 0; i < final_text_length; i++, j = j + 2) {
        text[i] = (((hexValue[j] % 32 + 9) % 25) * 16) + (((hexValue[j+1] % 32) + 9) % 25);
    }

    return text;
}

std::string HexUtil::textToHex(const std::string &text) {

    std::string hexValue = std::string(text.size() << 1, ' ');
    char hexMap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    for (size_t i = 0; i < text.size(); i++) {
        hexValue[2 * i] =  hexMap[(text[i] & 0xF0) >> 4];
        hexValue[2 * i + 1] = hexMap[text[i] & 0x0F];
    }

    return hexValue;
}
