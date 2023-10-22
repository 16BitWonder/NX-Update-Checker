#include "main.hpp"
#include "common.hpp"

enum UISelection {
	printAvailableUpdates,
	printLocalVersions,
	updateExternalVersions,
	manageCartridgeVersions
};

namespace ui {
	class LoadingBar {
		private:
		const int STEPS = 20;
		int total = 0;
		
		public:
		LoadingBar(int totalEntries) { total = totalEntries; }
		void updateLoadingBar(int);
	};
	
	void printHeader();
	void updateUI(u64);
	u8 getSelection();
}
