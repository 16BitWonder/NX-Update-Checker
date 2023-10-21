#include "main.hpp"
#include "common.hpp"
#include "ui.hpp"
#include "VersionDB.hpp"

int main(int argc, char **argv) {
	
	enum AppState {
		mainMenu
	};
	
	// Init controller
	PadState pad;
	padConfigureInput(1, HidNpadStyleSet_NpadStandard);
	padInitializeDefault(&pad);
	consoleInit(NULL);
	
	// Begin appletMainLoop
	while(appletMainLoop()) {
		
		ui::updateUI();
		
		padUpdate(&pad);
		u64 kDown = padGetButtonsDown(&pad);
		if (kDown & HidNpadButton_Plus) break;
		consoleUpdate(NULL);
		consoleClear();
	}

	// Free what's left before return
	consoleExit(NULL);
	return 0;
}