#include "VersionDB.hpp"
#include <cstdlib>
#include <cstring>

// Private Functions

// Navigates traverse to the Entry with a matching id and returns true, otherwise false
bool VersionDB::navigateToApplicationID(u64 application_id) {
	
	// Move to head to begin search
	traverse = head;
	
	// Search through this VersionDB for a matching id
	for (int i = 0; i < size ; i++) {
		if (traverse->id == application_id) {
			return true;
		}
		traverse = traverse->next;
	}
	
	return false;
}

// Public Functions

// Takes provided title info, creates an Entry and appends it to this VersionDB at the tail - name and displayVersion are optional
void VersionDB::addTitle(u64 application_id, u32 version, char name[0x200], char displayVersion[0x10]) {
	
	//Allocate new Entry
	traverse = (Entry*)malloc(sizeof(Entry));
	
	// Set head/tail if this is the first Entry
	if (head == NULL) {
		head = tail = traverse;
	} else {
		tail->next = traverse;
		traverse->prev = tail;
		tail = traverse;
	}
	
	// Copy guarunteed information
	tail->id = application_id;
	tail->version = version;
	
	// Copy optional information
	if (name != NULL) {
		memcpy(tail->name, name, strlen(name)+1);
	}
	if (displayVersion != NULL) {
		memcpy(tail->displayVersion, displayVersion, strlen(displayVersion)+1);
	}
	
	size++;
	return;
}

// Overload of addTitle allowing std::string to be passed instead of char arrays
void VersionDB::addTitleStrings(u64 application_id, u32 version, std::string name, std::string displayVersion) {
	
	// Convert std::string args to char[...]
	char* convertedName = NULL;
	char* convertedDisplayVersion = NULL;
	if (!name.empty()) {
		convertedName = (char*)malloc(0x200 * sizeof(char));
		std::strcpy(convertedName, name.c_str());
	}
	if (!displayVersion.empty()) {
		convertedDisplayVersion = (char*)malloc(0x10 * sizeof(char));
		std::strcpy(convertedDisplayVersion, displayVersion.c_str());
	}
	
	// Call into normal addTitle
	addTitle(application_id, version, convertedName, convertedDisplayVersion);	
	
	return;
}

// Takes provided application_id and version, and updates the stored version in this VersionDB if the new version is greater
// Returns true if the title was found (regardless of whether it was updated) and false if it was not
bool VersionDB::updateTitleVersion(u64 application_id, u32 version) {
	
	// Move to head to begin search
	traverse = head;
	
	// Search through this VersionDB and update the stored version if necessary
	if (VersionDB::navigateToApplicationID(application_id)) {
		if (traverse->version < version) {
			traverse->version = version;
		}
		return true;
	}
	
	return false;
}