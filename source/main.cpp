#include "main.hpp"
#include "common.hpp"
#include "Controller.hpp"
#include "ns.hpp"
#include "ui.hpp"
#include "VersionDB.hpp"
#include <string>

int main(int argc, char **argv) {
	
	// Init console
	consoleInit(NULL);
	
	// Init Controller
	Controller controller;
	controller.Init();
	
	// Begin appletMainLoop
	VersionDB localVersions;
	VersionDB externalVersions;
	VersionDB cartVersions;
	bool runOnStartup = true;
	while(appletMainLoop()) {
		
		// Init Local and External Version Lists
		if (runOnStartup) {
			// TODO
			// Update cartridge version List file with inserted cartridge
			// Init localVersions
			cartVersions = ns::initExternalVersions("cart_versions.txt");
			// Update cartVersions with current cart info if necessary
			// Update localVersions with cartVersions
			externalVersions = ns::initExternalVersions("versions.txt");
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