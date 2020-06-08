#include "main.h"

void initLists(NsApplicationRecord **titleRecords, int *recordsLength, NsApplicationContentMetaStatus ***metaStatusList, int **metaLength)
{
	nsInitialize();
	/* Construct Installed Titles List */
    NsApplicationRecord *wipRecords;
	wipRecords = malloc(sizeof(NsApplicationRecord) * 5000);
    nsListApplicationRecord(wipRecords, 5000, 0, recordsLength);
	
	/* Construct Meta Status List */
	int tmpRecordsLength = *recordsLength;
	int *wipMetaLength;
	NsApplicationContentMetaStatus **wipMetaStatusList;
	wipMetaLength = malloc(sizeof(int) * tmpRecordsLength);
	wipMetaStatusList = malloc(sizeof(NsApplicationContentMetaStatus*) * tmpRecordsLength);
	for (int i = 0; i < tmpRecordsLength; i++)
	{
		*(wipMetaLength+i) = 0;
		*(wipMetaStatusList+i) = malloc(sizeof(NsApplicationContentMetaStatus) * 30);
		nsListApplicationContentMetaStatus(wipRecords[i].application_id, 0, *(wipMetaStatusList+i), 10, wipMetaLength+i);
	}
	nsExit();
	
	*metaStatusList = wipMetaStatusList;
	*metaLength = wipMetaLength;
	*titleRecords = wipRecords;
	
	return;
}