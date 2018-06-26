#include <string>
#include "dmx_custom_functions.h"
#include "HexUtil.h"


DMX_CUSTOM_FUNCTION(HexToText, DMX_STRING(text), DMX_STRING(input)) {

    if (input.isNull()) {
        text.setNull();
    }
    else {
        //Hex conversion
        text = HexUtil::hexToText(input);
    }

    return DMX_CUSTOM_FUNCTION_SUCCESS;
}

DMX_CUSTOM_FUNCTION(TextToHex, DMX_STRING(text), DMX_STRING(input)) {

    if (input.isNull()) {
        text.setNull();
    }
    else {
        //Hex conversion
        text = HexUtil::textToHex(input);
    }

    return DMX_CUSTOM_FUNCTION_SUCCESS;
}

