/*******************************************************************************

 Copyright (c) 2017-present

 *******************************************************************************/
#include <string>
#include "StringUtil.h"
#include <algorithm>

/******************************************************************************/

std::string StringUtil::stringReverse(const std::string &text) {
    //create a copy
	std::string copyText(text);
	std::reverse(copyText.begin(), copyText.end());
	return copyText;
}
