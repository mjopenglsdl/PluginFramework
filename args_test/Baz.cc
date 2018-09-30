// This file includes a sample plugin using the plugin framework.
//

#include <string>
#include "plugin.h"

#include "takesArg.hh"

PLUGIN(takesArg,Baz) {
    private:
	std::string arg;

    public:
	~Baz() {}
	Baz(std::string a) : arg(a) {}
	Baz() : arg("Undef") {}

	// Define the plugin methods.
	std::string print(void) {
	    return "Baz::" + arg;
	}
};

/* Magic? */
PLUGIN_DEFS(takesArg,Baz,std::string)
PLUGIN_DEFS(takesArg,Baz)
