#include <switch.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
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

typedef struct CartEntry{
	u64 patch_id;
	s32 version;
} CartEntry;

void updateStoredCartVerList(void);
int getCartVerListCount(void);
CartEntry** initCartVerList(int);
bool downloadWebVerList(void);
bool initLogging(void);
void initLists(NsApplicationRecord**, int*, NsApplicationContentMetaStatus***, int**);
void updateMeta(char**, char**, u64);
Entry* initLocalVerList(CartEntry**, int);
Entry* initExtVerList(void);
Entry* handleVerList(Entry*, long, char*);
void checkForUpdates(FILE*, Entry*, Entry*);
void freeList(Entry*);