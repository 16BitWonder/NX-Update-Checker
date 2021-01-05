#include "main.h"

bool logging = false;
PadState pad;

int main(int argc, char **argv)
{
	logging = initLogging();
	
	padConfigureInput(1, HidNpadStyleSet_NpadStandard);
	padInitializeDefault(&pad);
	
	consoleInit(NULL);
	
	Entry *localVerList;
	localVerList = initLocalVerList();
	Entry *currLocalEntry = localVerList->next;
	
	socketInitializeDefault();
	Entry *extVerList;
	extVerList = initWebVerList();
	socketExit();

	if (extVerList == NULL){
		printf("\nPress (+) to continue.\n");
		consoleUpdate(NULL);
		while (1)
		{
			padUpdate(&pad);
			u64 kDown = padGetButtonsDown(&pad);
			if (kDown & HidNpadButton_Plus)
				break;
		}
		consoleClear();
		extVerList = initExtVerList();
	}
	
	if (extVerList == NULL)
	{
		freeList(localVerList);
		consoleExit(NULL);
		return 0;
	}
	printf("versions.txt loaded!\n\n");
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
			padUpdate(&pad);
			u64 kDown = padGetButtonsDown(&pad);
			if (kDown & HidNpadButton_Plus) return 0;
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
					padUpdate(&pad);
					u64 kDown = padGetButtonsDown(&pad);
					if (kDown & HidNpadButton_Plus)
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
			if (logging)
			{
				printf("\nLogging was enabled");
			}
			else
			{
				printf("\nLogging was not enabled");
			}
			finished = true;
		}
		
		padUpdate(&pad);
		u64 kDown = padGetButtonsDown(&pad);
		if (kDown & HidNpadButton_Plus) break;
		consoleUpdate(NULL);
    }

	freeList(localVerList);
	freeList(extVerList);
	consoleExit(NULL);
    return 0;
}