#include "main.h"

void checkForUpdates(FILE *out, Entry *localEntries, Entry *extEntries)
{
	Entry *tmpLocal = localEntries->next;
	Entry *tmpExt;
	
	fprintf(out, "Updates available:\n\n");
	
	while (tmpLocal != NULL)
	{
		tmpExt = extEntries->next;
		while (tmpExt != NULL)
		{
			if (strcmp(tmpExt->Data.TID, tmpLocal->Data.TID) == 0)
			{
				if (tmpExt->Data.version > tmpLocal->Data.version)
				{
					fprintf(out, "%s [0100%s000][%s][v%d] -> [v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.displayVersion, tmpLocal->Data.version, tmpExt->Data.version);
					printf("%s [0100%s000][%s][v%d] -> [v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.displayVersion, tmpLocal->Data.version, tmpExt->Data.version);
					consoleUpdate(NULL);
				}
				tmpExt = NULL;
			}
			else
			{
				tmpExt = tmpExt->next;
			}
		}
		tmpLocal = tmpLocal->next;
	}
    return;
}