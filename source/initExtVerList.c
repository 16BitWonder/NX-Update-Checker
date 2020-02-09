#include "main.h"

Entry* initExtVerList()
{
	FILE *verListJson;
	Entry *listHead, *currExtEntry, *tmp;
	char *memVerList;
	
	/* Have an empty entry at head of list */
	currExtEntry = calloc(1, sizeof(Entry));
	listHead = currExtEntry;
	
	verListJson = fopen("versionlist.json", "r");
	if (verListJson == NULL)
	{
		perror("versionlist.json");
		printf("\nPlease place 'versionlist.json' in the same directory as this nro.\n\n");
		printf("You may find a compatible json this app was tested with on the ReSwitched\n");
		printf("Discord Server under #versionlist-updates.\n\n");
		printf("ReSwitched Discord Link: https://discord.gg/ZdqEhed\n\n");
		printf("Press (+) to exit.");
		consoleUpdate(NULL);
		while (1)
		{
			hidScanInput();
			u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
			if (kDown & KEY_PLUS) return NULL;
		}
	}
	
	fseek(verListJson, 0, SEEK_END);
	long fileSize = ftell(verListJson);
	memVerList = calloc(fileSize, sizeof(char));
	rewind(verListJson);
	fread(memVerList, 1, fileSize, verListJson);
	
	/*Close our input file*/
	if (fclose(verListJson) != 0)
	{
		perror("versionlist.json");
		printf("\nPress (+) to exit.");
		consoleUpdate(NULL);
		while (1)
		{
			hidScanInput();
			u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
			if (kDown & KEY_PLUS) return NULL;
		}
	}
	
	/* Create our linked list of versionlist Entries */
	long index = 18;
	int parsedExtEntries = 0;
	while (index < fileSize)
	{
		if (*(memVerList+index) == '0')
		{
			tmp = calloc(1, sizeof(Entry));
			tmp->prev = currExtEntry;
			currExtEntry->next = tmp;
			tmp = NULL;
			currExtEntry = currExtEntry->next;
			
			/* Parse TID into TID */
			index += 4;
			for (int i = 0; i < 9; i++)
			{
				currExtEntry->Data.TID[i] = *(memVerList + (index + i));
			}
			
			/* Parse version into version */
			index += 24;
			currExtEntry->Data.version = 0;
			while (*(memVerList+index) != ',')
			{
				currExtEntry->Data.version *= 10;
				currExtEntry->Data.version += (*(memVerList+(index)) - 48);
				index++;
			}
			
			/* Move to next title */
			index += 20;
			while (*(memVerList+index) != '}')
			{
				index++;
			}
			index += 9;
		}
		else
		{
			index = fileSize;
		}
		consoleClear();
		printf("Parsing versionlist.json...\n\n");
		printf("Parsed entries: %d", ++parsedExtEntries);
		consoleUpdate(NULL);
	}
	
	consoleClear();
	return listHead;
}