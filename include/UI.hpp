#include "main.hpp"
#include "common.hpp"

enum UISelection {
	printAvailableUpdates,
	printLocalVersions,
	updateExternalVersions,
	manageCartridgeDatabase
};

namespace ui {
	void updateUI(u64);
}