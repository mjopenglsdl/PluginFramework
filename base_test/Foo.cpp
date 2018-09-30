// This file includes a sample plugin using the plugin framework.
//

#include <string>
#include "plugin.h"

#include "CHasPrint.h"

PLUGIN(CHasPrint,Foo) {
public:
    ~Foo() {}

    // Define the plugin methods.
    std::string print(void) {
        return "Foo::print";
    }
};


PLUGIN_DEFS(CHasPrint, Foo);