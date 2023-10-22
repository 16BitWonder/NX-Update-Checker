#include "main.hpp"
#include "common.hpp"
#include "Controller.hpp"
#include "ui.hpp"
#include "VersionDB.hpp"

int main(int argc, char **argv) {
	
	// Init console
	consoleInit(NULL);
	
	// Init Controller
	Controller controller;
	controller.Init();
	
	// Begin appletMainLoop
	VersionDB localVersions;
	VersionDB externalVersions;
	bool runOnStartup = true;
	while(appletMainLoop()) {
		
		// Init Local and External Version Lists
		if (runOnStartup) {
			// TODO
			// Update cartridge version List file with inserted cartridge
			// Init localVersions
			// Declare/Init cartVersions
			// Update localVersions with cartVersions
			// Init externalVersions (should account for missing versions.txt)
			runOnStartup = false;
		}
		
		// Update input
		u64 kPressed = controller.getNewPressed();
		
		// Execute selection
		if (kPressed & HidNpadButton_A) {
			switch (ui::getSelection()) {
				case printAvailableUpdates:
					// TODO
					break;
				case printLocalVersions:
					// TODO
					break;
				case updateExternalVersions:
					// TODO
					break;
				case manageCartridgeVersions:
					// TODO
					break;
				default:
					break;
			}
		}
		
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