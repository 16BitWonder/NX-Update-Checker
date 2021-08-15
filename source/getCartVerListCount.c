#include "main.h"
int getGCDBCount(char* index);
char* readDBFileIntoMem();

int getCartVerListCount() {
	
	// Read the file into a char*
	char *gcDBMem = readDBFileIntoMem();
	
	// Count the number of entries and then malloc the required Cart_Entry size
	int currDBEntries = getGCDBCount(gcDBMem);
	
	free(gcDBMem);
	return currDBEntries;
}