#include <switch.h>
#include <stdio.h>
#include <mutex>

#define VERSIONS_URL "https://raw.githubusercontent.com/16BitWonder/nx-versions/master/versions.txt"

struct MetaData {
	char TID [17];
	int version;
	char displayVersion [0x10];
	char name [0x200];
};
struct Entry {
	struct MetaData Data;
	struct Entry *next;
	struct Entry *prev;
};
struct CartEntry{
	u64 patch_id;
	s32 version;
};