#include "main.h"

Entry *handleVerList(Entry *currExtEntry, long fileSize, char *memVerList)
{
	Entry *listHead, *tmp;
	listHead = currExtEntry;

	/* Create our linked list of versionlist Entries */
	int parsedExtEntries = 0;
	long index = 12;
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
			index += 17;

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
			index += 17;
			while (*(memVerList + index) != 13)
				index++;
		}
		index += 2;
		
		if(index % 250 == 0){
			consoleClear();
			printf("Parsing versions.txt...\n\n");
			printf("Parsed entries: %d", parsedExtEntries);
			consoleUpdate(NULL);
		}
	}

	consoleClear();
	return listHead;
}