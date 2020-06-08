#include "main.h"

Entry* initExtVerList()
{
	FILE *verListTxt;
	Entry *listHead, *currExtEntry, *tmp;
	char *memVerList;
	
	verListTxt = fopen("versions.txt", "r");
	if (verListTxt == NULL)
	{
		printf("\nPlease place a copy of versions.txt in the same directory as this nro.\n");
		printf("You may find a compatible versions.txt this app was tested with on the\n");
		printf("following repository: https://github.com/blawar/titledb/blob/master/versions.txt\n\n");
		printf("Press (+) to exit.");
		consoleUpdate(NULL);
		while (1)
		{
			hidScanInput();
			u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
			if (kDown & KEY_PLUS) return NULL;
		}
	}
	
	/* Have an empty entry at head of list */
	currExtEntry = calloc(1, sizeof(Entry));
	listHead = currExtEntry;
	
	char* selectedTxt = calloc(NAME_MAX, sizeof(char));
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
			hidScanInput();
			u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
			if (kDown & KEY_PLUS) return NULL;
		}
	}
	free(selectedTxt);
	
	/* Create our linked list of versionlist Entries */
	int parsedExtEntries = 0;
	long index = 21;
	while (index < fileSize)
	{
		if ((*(memVerList + (index +  13)) != 0) || (*(memVerList + (index + 14)) != 0) || (*(memVerList + (index + 15)) != 0))
		{
			tmp = calloc(1, sizeof(Entry));
			tmp->prev = currExtEntry;
			currExtEntry->next = tmp;
			tmp = NULL;
			currExtEntry = currExtEntry->next;

			/* Parse TID */
			for (int i = 0; i < 16; i++)
			{
				currExtEntry->Data.TID[i] = *(memVerList + (index + i));
			}
			currExtEntry->Data.TID[16] = 0;
			index += 50;

			/* Parse Version */
			char c = 0;
			currExtEntry->Data.version = 0;
			if (*(memVerList + index) != 13)
			{
				c = *(memVerList + index);
				while (c != 13)
				{
					currExtEntry->Data.version *= 10;
					currExtEntry->Data.version += (*(memVerList + index) - 48);
					index++;
					c = *(memVerList + index);
				}
			}
			parsedExtEntries++;
		}
		else
		{
			index += 50;
			while (*(memVerList + index) != 13)
				index++;
		}
		index += 2;
		
		consoleClear();
		printf("Parsing versions.txt...\n\n");
		printf("Parsed entries: %d", parsedExtEntries);
		consoleUpdate(NULL);
	}

	consoleClear();
	return listHead;
}