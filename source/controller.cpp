#include "Controller.hpp"

// Private Functions
	
// Public Functions

void Controller::Init() {
	// Init Controller
	padConfigureInput(1, HidNpadStyleSet_NpadStandard);
	padInitializeDefault(&pad);
}

u64 Controller::getNewPressed() {
	// Get kDown
	padUpdate(&pad);	
	u64 kDown = padGetButtonsDown(&pad);
	
	// Get kNewPressed
	u64 kNewPressed = (kHeld xor kDown) & kDown;
	
	// Update kHeld
	u64 kTmp = kHeld or kDown;
	kHeld = (kTmp xor kDown) xor kTmp;
	
	return kNewPressed;
}