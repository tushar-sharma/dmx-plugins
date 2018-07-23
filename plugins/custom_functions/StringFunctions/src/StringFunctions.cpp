#include <string>
#include "dmx_custom_functions.h"
#include "StringUtil.h"
#include <vector>

DMX_CUSTOM_FUNCTION(StringReverse, DMX_STRING(text), DMX_STRING(input)) {

    if (input.isNull()) {
        text.setNull();
    }
    else {
        // reverse a string
        text = StringUtil::stringReverse(input);
    }

    return DMX_CUSTOM_FUNCTION_SUCCESS;
}

DMX_CUSTOM_FUNCTION(FrequentWord, DMX_STRING(text), DMX_STRING(input)) {

    if (input.isNull()) {
        text.setNull();
    }
    else {
        // most frequent word
		text = StringUtil::frequentWord(input);
    }

    return DMX_CUSTOM_FUNCTION_SUCCESS;
}
