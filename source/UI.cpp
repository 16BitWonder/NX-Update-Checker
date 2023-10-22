#include "ui.hpp"
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
		printf("   Print Available Updates\n");
		printf("   Print Local Version List\n");
		printf("   Update External Version List\n");
		printf("   Manage Cartridge Version List\n");
		return;
	}
	
	void updateCursor(u64 kPressed) {
		// Update cursor
		if ((kPressed & HidNpadButton_Up) && (cursor > printAvailableUpdates)) {
			cursor--;
		}
		if ((kPressed & HidNpadButton_Down) && (cursor < manageCartridgeVersions)) {
			cursor++;
		}
		return;
	}
	
	void updateUI(u64 kPressed) {
		// Print MainMenu
		printMainMenu();
		
		// Update cursor
		updateCursor(kPressed);
		
		// Print cursor
		printf("\x1b[%d;2H>", 5+cursor);
	}
	
	u8 getSelection() {
		return cursor;
	}
}