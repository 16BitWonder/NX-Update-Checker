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
	
	/* Display Version of Current Title */
	char *titleDispVersion;

	int c  = 0;
	while (recordsEntry < recordsLength)
	{
		currMeta = *(metaStatusList+recordsEntry);
		updateMeta(&titleName, &titleDispVersion, titleRecords[recordsEntry].application_id);

		/* Parse through all content under current Title*/
		bool updateFound = false;
		bool baseFound = false;
		for (int i = 0; i < *(metaLength + recordsEntry); i++)
		{
			tmp = calloc(1, sizeof(Entry));
			currEntry->next = tmp;
			tmp->prev = currEntry;
			tmp = NULL;
			currEntry = currEntry->next;

			/* Parse TID into currEntry */
			char tmpTID[17];
			for (int j = 0; j < 16; j++)
			{
				char currDigit;
				int currInt = currMeta[i].application_id % 16;
				currMeta[i].application_id /= 16;
				if (currInt > 9)
				{
					currInt -= 10;
					currDigit = currInt + 'A';
				}
				else
				{
					currDigit = currInt + '0';
				}
				tmpTID[j] = currDigit;
			}

			/* Need to flip what we parsed */
			char tmpFlipTID[17];
			for (int j = 0; j < 8; j++)
			{
				tmpFlipTID[j] = tmpTID[15 - j];
				tmpFlipTID[15 - j] = tmpTID[j];
			}
			tmpFlipTID[16] = 0;

			/* Check what kind of content we have and parse accordingly */
			if (!updateFound && (tmpFlipTID[14] == '0') && (tmpFlipTID[15] == '0'))
			{
				/* Base Title */
				if (tmpFlipTID[13] == '0')
				{
					strcpy(currEntry->Data.TID, tmpFlipTID);
					currEntry->Data.TID[13] = '8';
					/* Parse Version into currEntry */
					currEntry->Data.version = currMeta[i].version;
					/* Parse Name into currEntry */
					strncpy(currEntry->Data.name, titleName, 0x200);
					/* Parse DispVersion into currEntry */
					strncpy(currEntry->Data.displayVersion, titleDispVersion, 0x0F);
					baseFound = true;
				}
				/* Update */
				else if (tmpFlipTID[13] == '8')
				{
					if (!baseFound)
					{
						strcpy(currEntry->Data.TID, tmpFlipTID);
						/* Parse Version into currEntry */
						currEntry->Data.version = currMeta[i].version;
						/* Parse Name into currEntry */
						strncpy(currEntry->Data.name, titleName, 0x200);
						/* Parse DispVersion into currEntry */
						strncpy(currEntry->Data.displayVersion, titleDispVersion, 0x0F);
					}
					else
					{
						currEntry = currEntry->prev;
						free(currEntry->next);
						tmp = currEntry;
						bool updatedBase = false;
						while (!updatedBase)
						{
							if (tmp->Data.TID[13] == '8')
							{
								/* Update Version in tmp */
								tmp->Data.version = currMeta[i].version;
								updatedBase = true;
							}
							tmp = tmp->prev;
						}
						tmp = NULL;
					}
					updateFound = true;
				}
			}
			/* DLC */
			else
			{
				strcpy(currEntry->Data.TID, tmpFlipTID);
				/* Parse Version into currEntry */
				currEntry->Data.version = currMeta[i].version;
				/* Parse Name into currEntry */
				strncpy(currEntry->Data.name, titleName, 0x200);
				/* Parse DispVersion into currEntry */
				strncpy(currEntry->Data.displayVersion, titleDispVersion, 0x0F);
			}

			printf("Building Local VerList: %d", ++c);
			consoleUpdate(NULL);
			consoleClear();
		}
		
		/* Move to next record */
		recordsEntry++;
	}
	
	free(titleRecords);
	free(metaLength);
	free(metaStatusList);
	return localVerList;
}