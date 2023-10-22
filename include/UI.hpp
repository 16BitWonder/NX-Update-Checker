#include "main.hpp"
#include "common.hpp"

enum UISelection {
	printAvailableUpdates,
	printLocalVersions,
	updateExternalVersions,
	manageCartridgeVersions
};

namespace ui {
	void printHeader();
	void updateUI(u64);
	u8 getSelection();
}