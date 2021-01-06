#include "main.h"

void freeList(Entry *head) {
	Entry *currEntry;
	currEntry = head;
	
	/* Free List */
	while (head != NULL) {
		head = head->next;
		currEntry->prev = NULL;
		currEntry->next = NULL;
		free(currEntry);
		currEntry = head;
	}
	return;
}