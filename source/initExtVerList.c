#include "main.h"

Entry* initExtVerList()
{
	FILE *verListJson;
	Entry *listHead, *currExtEntry, *tmp;
	char *memVerList;

	char workingDir[FILENAME_MAX];
	GetCurrentDir(workingDir, FILENAME_MAX);
	
	char **jsonFiles;
	int totalJson = 0;
	jsonFiles = (char**)calloc(40, sizeof(char*));
	DIR *dir;
	struct dirent *entry;
	if ((dir = opendir (workingDir)) != NULL)
	{
		while((totalJson < 40) && ((entry = readdir (dir)) != NULL))
		{
			if(strstr(entry->d_name, ".json") != NULL)
			{
				*(jsonFiles+totalJson) = (char*)calloc(NAME_MAX, sizeof(char));
				strcpy(*(jsonFiles+totalJson), entry->d_name);
				totalJson++;
			}
		}
		closedir (dir);
	}
	
	if (totalJson == 0)
	{
		printf("\nPlease place a versionlist JSON in the same directory as this nro.\n\n");
		printf("You may find a compatible JSON this app was tested with on the ReSwitched\n");
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
	
	char *selectedJson;
	selectedJson = calloc(NAME_MAX, sizeof(char));
	int selection = 0;
	while (totalJson != -1)
	{
		printf("Select a JSON\n");
		for (int i = 1; i < (totalJson+1); i++)
		{
			printf("\x1b[%d;4H%s", (i+2), *(jsonFiles+(i-1)));
		}
		printf("\x1b[%d;2H>", (selection+3));
		
		hidScanInput();
		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
		
		if (kDown & KEY_PLUS)
		{
			free(selectedJson);
			free(jsonFiles);
			return NULL;
		}
		if ((kDown & KEY_DUP) || (kDown & KEY_LSTICK_UP))
		{
			if (--selection < 0)
			{
				selection = totalJson - 1;
			}
		}
		if ((kDown & KEY_DDOWN) || (kDown & KEY_LSTICK_DOWN))
		{
			if (++selection == totalJson)
			{
				selection = 0;
			}
		}
		if (kDown & KEY_A)
		{
			totalJson = -1;
		}
		
		consoleUpdate(NULL);
		consoleClear();
	}
	strcpy(selectedJson, *(jsonFiles+selection));
	free(jsonFiles);
	
	verListJson = fopen(selectedJson, "r");
	if (verListJson == NULL)
	{
		perror(selectedJson);
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
	
	fseek(verListJson, 0, SEEK_END);
	long fileSize = ftell(verListJson);
	memVerList = calloc(fileSize, sizeof(char));
	rewind(verListJson);
	fread(memVerList, 1, fileSize, verListJson);
	
	/*Close our input file*/
	if (fclose(verListJson) != 0)
	{
		perror(selectedJson);
		printf("\nPress (+) to exit.");
		consoleUpdate(NULL);
		while (1)
		{
			hidScanInput();
			u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
			if (kDown & KEY_PLUS) return NULL;
		}
	}
	free(selectedJson);
	
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
		printf("Parsing JSON...\n\n");
		printf("Parsed entries: %d", ++parsedExtEntries);
		consoleUpdate(NULL);
	}
	
	consoleClear();
	return listHead;
}