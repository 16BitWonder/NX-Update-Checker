#include <string.h>
#include <stdlib.h>
#include "main.h"

void updateMeta(char **namePtr, char **dispVerPtr, u64 TID)
{
	nsInitialize();
	char *name;
	char *dispVer;
	name = calloc(0x201, sizeof(char));
	dispVer = calloc(0x10, sizeof(char));
	NacpLanguageEntry *nacpName;
	NsApplicationControlData* titleControl;
	titleControl = malloc(sizeof(NsApplicationControlData));
	u64 length = 0;
	nsGetApplicationControlData(NsApplicationControlSource_Storage, TID, titleControl, sizeof(NsApplicationControlData), &length);
	nacpGetLanguageEntry(&titleControl->nacp, &nacpName);
	strncpy(name, nacpName->name, 0x201);
	strncpy(dispVer, titleControl->nacp.display_version, 0x10);
	nsExit();
	
	*(namePtr) = name;
	*(dispVerPtr) = dispVer;
	return;
}