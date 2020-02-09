#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <switch.h>

struct MetaData {
	char TID [10];
	int version;
	char name [0x201];
};
typedef struct Entry {
	struct MetaData Data;
	struct Entry *next;
	struct Entry *prev;
}Entry;

void initLists(NsApplicationRecord**, int*, NsApplicationContentMetaStatus***, int**);
void updateTitleName(char**, u64);
Entry* initLocalVerList(void);
Entry* initExtVerList(void);
void checkForUpdates(FILE*, Entry*, Entry*);
void freeList(Entry*);