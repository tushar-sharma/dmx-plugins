#include <iostream>
#include "dmx_custom_functions.h"


DMX_CUSTOM_FUNCTION(HexToString, DMX_STRING(text), DMX_STRING(input)) {
    if (input.isNull()) {
        text.setNull();
    }
    else {
        //Hex conversion
    }

    return DMX_CUSTOM_FUNCTION_SUCCESS;
}

