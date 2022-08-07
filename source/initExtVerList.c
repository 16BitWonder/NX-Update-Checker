#include "main.h"

extern bool logging;
extern FILE *logFile;
extern PadState mainPad;

Entry* initExtVerList() {
	FILE *verListTxt;
	Entry *currExtEntry;
	char *memVerList;
	
	if (logging) {
		fprintf(logFile, "[initExtVerList] Attempting to open versions.txt\n");
		fflush(logFile);
	}
	verListTxt = fopen("versions.txt", "r");
	if (verListTxt == NULL) {
		if (logging) {
			fprintf(logFile, "[initExtVerList] versions.txt could not be opened\n");
			fflush(logFile);
		}
		printf("\nPlease place a copy of versions.txt in the same directory as this nro.\n");
		printf("You may find a compatible versions.txt this app was tested with on the\n");
		printf("following repository:\n");
		printf("%s\n\n", VERSIONS_URL);
		printf("Press (+) to exit.");
		consoleUpdate(NULL);
		while (1) {
			padUpdate(&mainPad);
			u64 kDown = padGetButtonsDown(&mainPad);
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
		fflush(logFile);
	}
	
	/*Close our input file*/
	if (fclose(verListTxt) != 0) {
		perror("versions.txt");
		printf("\nPress (+) to exit.");
		consoleUpdate(NULL);
		while (1) {
			padUpdate(&mainPad);
			u64 kDown = padGetButtonsDown(&mainPad);
			if (kDown & HidNpadButton_Plus) return NULL;
		}
	}
	if (logging) {
		fprintf(logFile, "[initExtVerList] Closed versions.txt\n");
		fflush(logFile);
	}
	
	Entry *ret = handleVerList(currExtEntry, fileSize, memVerList);
	free(memVerList);
	if (logging) {
		fprintf(logFile, "[initExtVerList] Freed memVerList\n");
		fflush(logFile);
	}
	return ret;
}
