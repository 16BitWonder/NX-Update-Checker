#include "ui.hpp"
#include <cstdio>

namespace ui {
	void printHeader() {
		printf("============================\n");
		printf("= NX-Update-Checker v%d.%d.%d =\n", VERSION_MAJOR, VERSION_MINOR, VERSION_BUGFIX);
		printf("============================\n\n");
		return;
	}
	
	void printMainMenu() {
		printHeader();
		printf("   Print Available-Updates\n");
		printf("   Print Local Version List\n");
		printf("   Update External Version List\n");
		printf("   Manage CartridgeDB\n");
		return;
	}
}