#include "common.hpp"

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
	int size = 0;
	bool navigateToApplicationID(u64);
	
	public:
	void addTitle(u64, u32, char [0x200], char [0x10]);
	bool updateTitleVersion(u64, u32);
};