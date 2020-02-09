#include "main.h"

int main(int argc, char **argv)
{
	consoleInit(NULL);
	
	Entry *localVerList;
	localVerList = initLocalVerList();
	Entry *currLocalEntry = localVerList->next;
	
	Entry *extVerList;
	extVerList = initExtVerList();
	if (extVerList == NULL)
	{
		freeList(localVerList);
		consoleExit(NULL);
		return 0;
	}
	printf("versionlist.json loaded!\n\n");
	printf("Checking against installed titles for updates...\n\n");
	
	FILE *updFile;
	updFile = fopen("/Available-Updates.txt", "w");
	if (updFile == NULL)
	{
		perror("Available-Updates.txt");
		printf("\nPress (+) to exit.");
		consoleUpdate(NULL);
		freeList(localVerList);
		freeList(extVerList);
		while (1)
		{
			hidScanInput();
			u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
			if (kDown & KEY_PLUS) return 0;
		}
	}
	
	bool finished = false;
    while(appletMainLoop())
	{
		
		if (!finished)
		{
			checkForUpdates(updFile, currLocalEntry, extVerList);
			
			if (fclose(updFile) != 0)
			{
				perror("Available-Updates.txt");
				printf("\nPress (+) to exit.");
				consoleUpdate(NULL);
				while (1)
				{
					hidScanInput();
					u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
					if (kDown & KEY_PLUS)
					{
						consoleExit(NULL);
						freeList(localVerList);
						freeList(extVerList);
						return 0;
					}
				}
			}
			
			printf("\nResults printed into sdmc:/Available-Updates.txt!\n\n");
			printf("Press (+) to exit.");
			finished = true;
		}
		
		hidScanInput();
		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
		if (kDown & KEY_PLUS) break;
		consoleUpdate(NULL);
    }

	freeList(localVerList);
	freeList(extVerList);
	consoleExit(NULL);
    return 0;
}