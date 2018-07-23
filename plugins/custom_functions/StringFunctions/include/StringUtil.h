#ifndef HexUtil_h
#define HexUtil_h
/*******************************************************************************

 Copyright (c) 2017-present

 *******************************************************************************/
#include <string>
#include <vector>
/******************************************************************************/
/* Custom function return statuses */

class StringUtil
{
public:
    // reverse a string
    static std::string stringReverse(const std::string& text);
    // most frequent word
    static std::string frequentWord(const std::string& text);
};

#endif /* StringUtil_h */
