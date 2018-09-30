// This file demonstrates how to use the plugin framework in the main
// application.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "plugin.h"
#include "CHasPrint.h"

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char *argv[])
{
    CHasPrint *p = nullptr;

    // if (argc < 2) {
	// cerr << "Usage: " << argv[0] << " <PluginName>" << endl;
	// return EXIT_FAILURE;
    // }

    // for (int i = 1; i < argc; ++i) {
	//     try {
	// 	    // Autoload
	// 	    p = Plugin<CHasPrint>::getPlugin(argv[i]);
	//     } catch (std::runtime_error &e) {
	// 	    cerr << "Failed to load plugin: " << e.what() << endl;
	// 	    exit(EXIT_FAILURE);
	//     }

	//     cout << p->print() << endl;
    // }


	try {
		// Autoload
		p = Plugin<CHasPrint>::getPlugin("Bar");
	} catch (std::runtime_error &e) {
		cerr << "Failed to load plugin: " << e.what() << endl;
		exit(EXIT_FAILURE);
	}

	cout << p->print() << endl;

    return 0;
}