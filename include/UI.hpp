#include "main.hpp"

enum UISelection {
	printAvailableUpdates,
	printLocalVersions,
	updateExternalVersions,
	manageCartridgeDatabase
};

namespace ui {
	void updateUI();
}