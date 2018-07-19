#include <string>
#include "dmx_custom_functions.h"
#include "StringUtil.h"


DMX_CUSTOM_FUNCTION(StringReverse, DMX_STRING(text), DMX_STRING(input)) {

    if (input.isNull()) {
        text.setNull();
    }
    else {
        //Hex conversion
        text = StringUtil::stringReverse(input);
    }

    return DMX_CUSTOM_FUNCTION_SUCCESS;
}