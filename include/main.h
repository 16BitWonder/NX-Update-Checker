#include <switch.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#define GetCurrentDir getcwd

#define VERSIONS_URL "https://raw.githubusercontent.com/16BitWonder/nx-versions/master/versions.txt"

struct MetaData {
	char TID [17];
	int version;
	char displayVersion [0x10];
	char name [0x201];
};
typedef struct Entry {
	struct MetaData Data;
	struct Entry *next;
	struct Entry *prev;
}Entry;

void initLists(NsApplicationRecord**, int*, NsApplicationContentMetaStatus***, int**);
void updateMeta(char**, char**, u64);
Entry* initLocalVerList(void);
Entry* initExtVerList(void);
Entry* initWebVerList(void);
Entry* handleVerList(Entry*, long, char*);
void checkForUpdates(FILE*, Entry*, Entry*);
void freeList(Entry*);