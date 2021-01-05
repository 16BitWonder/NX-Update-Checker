#include "main.h"

extern PadState pad;

Entry* initExtVerList()
{
	FILE *verListTxt;
	Entry *currExtEntry;
	char *memVerList;
	
	verListTxt = fopen("versions.txt", "r");
	if (verListTxt == NULL)
	{
		printf("\nPlease place a copy of versions.txt in the same directory as this nro.\n");
		printf("You may find a compatible versions.txt this app was tested with on the\n");
		printf("following repository:\n");
		printf("   %s\n\n", VERSIONS_URL);
		printf("Press (+) to exit.");
		consoleUpdate(NULL);
		while (1)
		{
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
	
	/*Close our input file*/
	if (fclose(verListTxt) != 0)
	{
		perror("versions.txt");
		printf("\nPress (+) to exit.");
		consoleUpdate(NULL);
		while (1)
		{
			padUpdate(&pad);
			u64 kDown = padGetButtonsDown(&pad);
			if (kDown & HidNpadButton_Plus) return NULL;
		}
	}
	
	return handleVerList(currExtEntry, fileSize, memVerList);
}
