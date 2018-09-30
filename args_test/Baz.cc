// This file includes a sample plugin using the plugin framework.
//

#include <string>
#include "plugin.h"

#include "takesArg.hh"

TYPE_PLUGIN(takesArg,Baz) {
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
DECLARE_PLUGIN(takesArg,Baz,std::string)
DECLARE_PLUGIN(takesArg,Baz)
