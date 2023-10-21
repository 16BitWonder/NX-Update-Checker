#include "main.hpp"
#include "common.hpp"
#include "Controller.hpp"
#include "ui.hpp"
#include "VersionDB.hpp"

int main(int argc, char **argv) {
	
	enum AppState {
		mainMenu
	};
	
	// Init console
	consoleInit(NULL);
	
	// Init Controller
	Controller controller;
	controller.Init();
	
	// Begin appletMainLoop
	while(appletMainLoop()) {
		
		// Update input
		u64 kPressed = controller.getNewPressed();
		
		// Update UI
		ui::updateUI(kPressed);
		
		// Check for exit
		if (kPressed & HidNpadButton_Plus) break;
		consoleUpdate(NULL);
		consoleClear();
	}

	// Free what's left before return
	
	// Close console
	consoleExit(NULL);
	return 0;
}