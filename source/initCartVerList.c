#include "main.h"

extern bool logging;
extern FILE *logFile;
extern PadState pad;

int getGCDBCount(char* index);
char* readDBFileIntoMem();

CartEntry** initCartVerList(int entries) {
	
	// Create CartVerList
	CartEntry** cartVerList = malloc(entries*sizeof(CartEntry*));
	
	// Read the file into a char*
	char *gcDBMem = readDBFileIntoMem();
	char *gcDBMemIndex = gcDBMem;
	
	// Fill out the entries
	char tmpTid[17];
	tmpTid[16] = '\0';
	for (int i = 0; i < entries; i++) {
		
		// Malloc the current entry
		cartVerList[i] = malloc(sizeof(CartEntry));
		
		// Get the next title ID
		strncpy(tmpTid, gcDBMemIndex, 16);
		cartVerList[i]->patch_id = strtol(tmpTid, NULL, 16);
		gcDBMemIndex += 17;
		
		// Get the next version
		cartVerList[i]->version = 0;
		while (*gcDBMemIndex != '\n') {
			cartVerList[i]->version *= 10;
			cartVerList[i]->version += *gcDBMemIndex - '0';
			gcDBMemIndex++;
		}
		gcDBMemIndex++;
	}
	free(gcDBMem);
	
	return cartVerList;
}
