#include <switch.h>
#include <stdio.h>
#include <mutex>

#define VERSIONS_URL "https://raw.githubusercontent.com/16BitWonder/nx-versions/master/versions.txt"
#define VERSION 2.0.0

struct Entry {
	u64 id;
	u32 version;
	char displayVersion [0x10];
	char name [0x200];
	struct Entry *next;
	struct Entry *prev;
};