#include <string>
#include "dmx_custom_functions.h"
#include "HexUtil.h"


DMX_CUSTOM_FUNCTION(HexToString, DMX_STRING(text), DMX_STRING(input)) {
    if (input.isNull()) {
        text.setNull();
    }
    else {
        //Hex conversion
        text = HexUtil::HextToText(input);
    }

    return DMX_CUSTOM_FUNCTION_SUCCESS;
}

