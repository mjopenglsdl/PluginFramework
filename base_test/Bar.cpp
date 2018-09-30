// This file includes a sample plugin using the plugin framework.
//

#include <string>
#include "plugin.h"

#include "CHasPrint.h"

TYPE_PLUGIN(CHasPrint,Bar) {
public:
    ~Bar() {}

    // Define the plugin methods.
    std::string print(void) {
        throw "111";
        return "Bar::print";
    }
};

DECLARE_PLUGIN(CHasPrint, Bar);