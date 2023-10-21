#include "main.hpp"
#include "common.hpp"
#include "controller.hpp"
#include "ui.hpp"
#include "VersionDB.hpp"

int main(int argc, char **argv) {
	
	enum AppState {
		mainMenu
	};
	
	// Init console
	consoleInit(NULL);
	
	// Init controller
	PadState pad;
	padConfigureInput(1, HidNpadStyleSet_NpadStandard);
	padInitializeDefault(&pad);
	
	// Begin appletMainLoop
	while(appletMainLoop()) {
		
		// Update UI
		ui::updateUI();
		
		// Update input
		u64 kDown = controller::getPadDown(pad);
		
		// Check for exit
		if (kDown & HidNpadButton_Plus) break;
		consoleUpdate(NULL);
		consoleClear();
	}

	// Free what's left before return
	
	// Close console
	consoleExit(NULL);
	return 0;
}