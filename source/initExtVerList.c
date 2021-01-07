#include "main.h"

extern bool logging;
extern FILE *logFile;
extern PadState pad;

Entry* initExtVerList() {
	FILE *verListTxt;
	Entry *currExtEntry;
	char *memVerList;
	
	if (logging) {
		fprintf(logFile, "[initExtVerList] Attempting to open versions.txt\n");
	}
	verListTxt = fopen("versions.txt", "r");
	if (verListTxt == NULL) {
		if (logging) {
			fprintf(logFile, "[initExtVerList] versions.txt could not be opened\n");
		}
		printf("\nPlease place a copy of versions.txt in the same directory as this nro.\n");
		printf("You may find a compatible versions.txt this app was tested with on the\n");
		printf("following repository:\n");
		printf("   %s\n\n", VERSIONS_URL);
		printf("Press (+) to exit.");
		consoleUpdate(NULL);
		while (1) {
			padUpdate(&pad);
			u64 kDown = padGetButtonsDown(&pad);
			if (kDown & HidNpadButton_Plus) return NULL;
		}
	}
	
	/* Have an empty entry at head of list */
	currExtEntry = calloc(1, sizeof(Entry));
	
	fseek(verListTxt, 0, SEEK_END);
	long fileSize = ftell(verListTxt);
	memVerList = calloc(fileSize, sizeof(char));
	rewind(verListTxt);
	fread(memVerList, 1, fileSize, verListTxt);
	if (logging) {
		fprintf(logFile, "[initExtVerList] Read versions.txt into memory\n");
	}
	
	/*Close our input file*/
	if (fclose(verListTxt) != 0) {
		perror("versions.txt");
		printf("\nPress (+) to exit.");
		consoleUpdate(NULL);
		while (1) {
			padUpdate(&pad);
			u64 kDown = padGetButtonsDown(&pad);
			if (kDown & HidNpadButton_Plus) return NULL;
		}
	}
	if (logging) {
		fprintf(logFile, "[initExtVerList] Closed versions.txt\n");
	}
	
	return handleVerList(currExtEntry, fileSize, memVerList);
}
