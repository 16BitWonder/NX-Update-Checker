#include "ui.hpp"
#include "common.hpp"
#include <cstdio>

namespace ui {
	u8 cursor = printAvailableUpdates;
	
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
	
	void updateCursor() {
		// TODO - Need hid implemented
		// update cursor value based on controller returned value
		return;
	}
	
	void updateUI() {
		// Print MainMenu
		printMainMenu();
		
		// Update cursor
		updateCursor();
		
		// Print cursor
		printf("\x1b[%d;2H>", 5+cursor);
	}
}