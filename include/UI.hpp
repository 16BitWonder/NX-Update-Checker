#include "main.hpp"
#include "common.hpp"

enum UISelection {
	printAvailableUpdates,
	printLocalVersions,
	updateExternalVersions,
	manageCartridgeVersions
};

namespace ui {
	void updateUI(u64);
}