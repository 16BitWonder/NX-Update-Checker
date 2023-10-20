#include "main.hpp"

PadState pad;

int main(int argc, char **argv) {
	
	// Init controller
	padConfigureInput(1, HidNpadStyleSet_NpadStandard);
	padInitializeDefault(&pad);
	consoleInit(NULL);
	
	// Update/Build local cartridge version database
	
	// Build local version database
	
	// Download versions.txt
	
	// Build external version database (return if missing)
	
	// Open Available-Updates.txt for writing
	
	// Begin appletMainLoop
	std::once_flag onceFlag;
    while(appletMainLoop()) {
		
		std::call_once(onceFlag, [](){
			// Check for updates
			
			// Close Available-Updates.txt
		});
		
		padUpdate(&pad);
		u64 kDown = padGetButtonsDown(&pad);
		if (kDown & HidNpadButton_Plus) break;
		consoleUpdate(NULL);
    }

	// Free what's left before return
	consoleExit(NULL);
    return 0;
}