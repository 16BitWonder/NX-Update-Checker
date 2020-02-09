#include <string.h>
#include <stdlib.h>
#include "main.h"

void updateTitleName(char **namePtr, u64 TID)
{
	nsInitialize();
	char *name;
	name = calloc(0x201, sizeof(char));
	NacpLanguageEntry *nacpName;
	NsApplicationControlData* titleControl;
	titleControl = malloc(sizeof(NsApplicationControlData));
	u64 length = 0;
	nsGetApplicationControlData(NsApplicationControlSource_Storage, TID, titleControl, sizeof(NsApplicationControlData), &length);
	nacpGetLanguageEntry(&titleControl->nacp, &nacpName);
	strncpy(name, nacpName->name, 0x200);
	nsExit();
	
	*(namePtr) = name;
	return;
}