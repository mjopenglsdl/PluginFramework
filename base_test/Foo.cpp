// This file includes a sample plugin using the plugin framework.
//

#include <string>
#include "plugin.h"

#include "CHasPrint.h"

TYPE_PLUGIN(CHasPrint,Foo) {
public:
    ~Foo() {}

    // Define the plugin methods.
    std::string print(void) {
        return "Foo::print";
    }
};


DECLARE_PLUGIN(CHasPrint, Foo);