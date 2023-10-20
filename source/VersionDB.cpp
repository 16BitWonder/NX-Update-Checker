#include "main.hpp"
#include <cstdlib>
#include <cstring>

struct Entry {
	u64 id;
	u32 version;
	char name [0x200];
	char displayVersion [0x10];
	struct Entry* next;
	struct Entry* prev;
};

class VersionDB {
	private:
	Entry* head;
	Entry* tail;
	Entry* traverse;
	int size;
	
	public:
	void addTitle(u64 application_id, u32 version, char name[0x200], char displayVersion [0x10]);
};

// Takes provided title info, creates an Entry and appends it to this VersionDB at the tail - name and displayVersion are optional
void VersionDB::addTitle(u64 application_id, u32 version, char name[0x200] = NULL, char displayVersion[0x10] = NULL) {
	
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
	
	return;
}