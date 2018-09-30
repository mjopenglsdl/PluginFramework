// This file includes a sample plugin using the plugin framework.
//

#include <string>
#include "plugin.h"

#include "CHasPrint.h"

PLUGIN(CHasPrint,Bar) {
public:
    ~Bar() {}

    // Define the plugin methods.
    std::string print(void) {
        throw "111";
        return "Bar::print";
    }
};

PLUGIN_DEFS(CHasPrint, Bar);
