#include "main.h"

bool logging = false;
FILE *logFile;
PadState pad;

int main(int argc, char **argv) {
	logging = initLogging();
	if (logging) {
		logFile = fopen("/NXUC.log", "w");
		fprintf(logFile, "[main] Logging Initialized\n");
	}
	
	padConfigureInput(1, HidNpadStyleSet_NpadStandard);
	padInitializeDefault(&pad);
	if (logging) {
		fprintf(logFile, "[main] Pad Initialized\n");
	}
	
	consoleInit(NULL);
	if (logging) {
		fprintf(logFile, "[main] Console Initialized\n");
	}
	
	Entry *localVerList;
	localVerList = initLocalVerList();
	Entry *currLocalEntry = localVerList->next;
	if (logging) {
		fprintf(logFile, "[main] Local Version List Initialized\n");
	}
	
	if (logging) {
		fprintf(logFile, "[main] Begin initWebVerList\n");
	}
	socketInitializeDefault();
	Entry *extVerList;
	extVerList = initWebVerList();
	socketExit();
	if (logging && (extVerList == NULL)) {
		fprintf(logFile, "[main] initWebVerList completed, did not retrieve versions.txt\n");
	} else if (logging) {
		fprintf(logFile, "[main] initWebVerList completed, retrieved versions.txt\n");
	}

	if (extVerList == NULL) {
		printf("\nPress (+) to continue.\n");
		consoleUpdate(NULL);
		while (1) {
			padUpdate(&pad);
			u64 kDown = padGetButtonsDown(&pad);
			if (kDown & HidNpadButton_Plus)
				break;
		}
		consoleClear();
		if (logging) {
			fprintf(logFile, "[main] Begin initExtVerList\n");
		}
		extVerList = initExtVerList();
		if (logging && (extVerList == NULL)) {
			fprintf(logFile, "[main] initExtVerList completed, did not find versions.txt\n");
		} else if (logging) {
			fprintf(logFile, "[main] initExtVerList completed, found versions.txt\n");
		}
	}
	
	if (extVerList == NULL) {
		freeList(localVerList);
		if (logging) {
			fprintf(logFile, "[main] Freed localVerList\n");
		}
		consoleExit(NULL);
		return 0;
	}
	if (logging) {
		fprintf(logFile, "[main] External Version List Initialized\n");
	}
	
	printf("versions.txt loaded!\n\n");
	printf("Checking against installed titles for updates...\n\n");
	
	FILE *updFile;
	updFile = fopen("/Available-Updates.txt", "w");
	if (updFile == NULL) {
		perror("Available-Updates.txt");
		printf("\nPress (+) to exit.");
		consoleUpdate(NULL);
		freeList(localVerList);
		freeList(extVerList);
		while (1) {
			padUpdate(&pad);
			u64 kDown = padGetButtonsDown(&pad);
			if (kDown & HidNpadButton_Plus) return 0;
		}
	}
	if (logging) {
		fprintf(logFile, "[main] Opened Available-Updates.txt\n");
	}
	
	bool finished = false;
    while(appletMainLoop()) {	
		if (!finished) {
			checkForUpdates(updFile, currLocalEntry, extVerList);
			if (logging) {
				fprintf(logFile, "[main] Finished checkForUpdates\n");
			}
			
			if (fclose(updFile) != 0) {
				perror("Available-Updates.txt");
				printf("\nPress (+) to exit.");
				consoleUpdate(NULL);
				while (1) {
					padUpdate(&pad);
					u64 kDown = padGetButtonsDown(&pad);
					if (kDown & HidNpadButton_Plus) {
						consoleExit(NULL);
						freeList(localVerList);
						freeList(extVerList);
						return 0;
					}
				}
			}
			if (logging) {
				fprintf(logFile, "[main] Closed Available-Updates.txt\n");
			}
			
			printf("\nResults printed into sdmc:/Available-Updates.txt!\n\n");
			printf("Press (+) to exit.\n");
			
			if (logging) {
				printf("\nLogging was enabled");
			}
			finished = true;
		}
		
		padUpdate(&pad);
		u64 kDown = padGetButtonsDown(&pad);
		if (kDown & HidNpadButton_Plus) break;
		consoleUpdate(NULL);
    }

	freeList(localVerList);
	if (logging) {
		fprintf(logFile, "[main] Freed localVerList\n");
	}
	freeList(extVerList);
	if (logging) {
		fprintf(logFile, "[main] Freed extVerList\n");
	}
	fclose(logFile);
	consoleExit(NULL);
    return 0;
}