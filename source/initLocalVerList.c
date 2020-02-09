#include "main.h"

Entry* initLocalVerList()
{
	/* Linked list of Entries to return */
	Entry *localVerList, *currEntry, *tmp;
	currEntry = calloc(1, sizeof(Entry));
	localVerList = currEntry;
	
	/* List of Installed Titles */
	int recordsEntry = 0;
    int recordsLength = 0;
    NsApplicationRecord *titleRecords;
	
	/* Meta Status Lists for Installed Titles */
	int *metaLength;
	NsApplicationContentMetaStatus **metaStatusList;
	
	/* Init titleRecords and metaStatusList */
	initLists(&titleRecords, &recordsLength, &metaStatusList, &metaLength);
	
	/* Meta Status List of Current Title */
	NsApplicationContentMetaStatus *currMeta;
	
	/* Name of Current Title */
	char *titleName;
	
	while (recordsEntry < recordsLength)
	{
		currMeta = *(metaStatusList+recordsEntry);
		updateTitleName(&titleName, titleRecords[recordsEntry].application_id);
		
		tmp = calloc(1, sizeof(Entry));
		currEntry->next = tmp;
		tmp->prev = currEntry;
		tmp = NULL;
		currEntry = currEntry->next;
		
		/* Parse TID into currEntry */
		char tmpTID [17];
		for (int i = 0; i < 16; i++)
		{
			char currDigit;
			int currInt = titleRecords[recordsEntry].application_id % 16;
			titleRecords[recordsEntry].application_id /= 16;
			if (currInt > 9)
			{
				currInt -= 10;
				currDigit = currInt + 'a';
			}
			else
			{
				currDigit = currInt + '0';
			}
			tmpTID[i] = currDigit;
		}
		/* Need to flip what we parsed */
		for (int i = 4; i < 8; i++)
		{
			currEntry->Data.TID[i - 4] = tmpTID[15 - i];
			currEntry->Data.TID[12 - i] = tmpTID[i - 1];
		}
		currEntry->Data.TID[4] = tmpTID[7];
		
		/* Parse Version into currEntry */
		bool foundUpdate = false;
		for (int i = 0; i < *(metaLength+recordsEntry); i++)
		{
			if (currMeta[i].meta_type == NcmContentMetaType_Patch) {
				currEntry->Data.version = currMeta[i].version;
				i = *(metaLength+recordsEntry);
				foundUpdate = true;
			}
		}
		if (!foundUpdate)
		{
			currEntry->Data.version = 0;
		}
		
		/* Parse Name into currEntry */
		strncpy(currEntry->Data.name, titleName, 0x200);
		
		/* Move to next record */
		recordsEntry++;
	}
	
	free(titleRecords);
	free(metaLength);
	free(metaStatusList);
	return localVerList;
}