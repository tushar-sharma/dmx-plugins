#ifndef HexUtil_h
#define HexUtil_h
/*******************************************************************************

 Copyright (c) 2017-present

 *******************************************************************************/
#include <string>
/******************************************************************************/
/* Custom function return statuses */

class HexUtil
{
public:
    // Convert a hex string to an ascii text string
    static std::string hexToText(const std::string& hexValue);

    // Convert a given text to hex string
    static std::string textToHex(const std::string& text);
};

#endif /* HexUtil_h */
