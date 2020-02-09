#include "main.h"

void freeList(Entry *head)
{
	Entry *currEntry;
	currEntry = head;
	
	/* Free List */
	while (currEntry != NULL)
	{
		head = head->next;
		free(currEntry);
		currEntry = head;
	}
	return;
}