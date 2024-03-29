#include "main.h"

bool logging = false;
FILE *logFile;
PadState pad;

int main(int argc, char **argv) {
	logging = initLogging();
	if (logging) {
		logFile = fopen("/NXUC.log", "w");
		fprintf(logFile, "[main] Logging Initialized\n");
		fflush(logFile);
	}
	
	padConfigureInput(1, HidNpadStyleSet_NpadStandard);
	padInitializeDefault(&pad);
	if (logging) {
		fprintf(logFile, "[main] Pad Initialized\n");
		fflush(logFile);
	}
	
	consoleInit(NULL);
	if (logging) {
		fprintf(logFile, "[main] Console Initialized\n");
		fflush(logFile);
	}
	
	updateStoredCartVerList();
	if (logging) {
		fprintf(logFile, "[main] Updated cart_versions.txt\n");
		fflush(logFile);
	}
	int cartEntryCount = getCartVerListCount();
	CartEntry **cartVerList = initCartVerList(cartEntryCount);
	if (logging) {
		fprintf(logFile, "[main] Cart Version List Initialized\n");
		fflush(logFile);
	}
	
	Entry *localVerList = initLocalVerList(cartVerList, cartEntryCount);
	if (logging) {
		fprintf(logFile, "[main] Local Version List Initialized\n");
		fflush(logFile);
	}
	free(cartVerList);
	
	if (logging) {
		fprintf(logFile, "[main] Begin downloadWebVerList\n");
		fflush(logFile);
	}
	bool downloadedVerList = downloadWebVerList();
	if (logging) {
		if(downloadedVerList) {
			fprintf(logFile, "[main] versions.txt successfully downloaded\n");
			fflush(logFile);
		} else {
			fprintf(logFile, "[main] Failed to download versions.txt\n");
			fflush(logFile);
		}
	}
	
	if (logging) {
		fprintf(logFile, "[main] Begin initExtVerList\n");
		fflush(logFile);
	}
	Entry *extVerList = initExtVerList();
	if (logging && (extVerList == NULL)) {
		fprintf(logFile, "[main] initExtVerList completed, did not find versions.txt\n");
		fflush(logFile);
	} else if (logging) {
		fprintf(logFile, "[main] initExtVerList completed, found versions.txt\n");
		fflush(logFile);
	}
	
	if (extVerList == NULL) {
		freeList(localVerList);
		if (logging) {
			fprintf(logFile, "[main] Freed localVerList\n");
			fflush(logFile);
		}
		consoleExit(NULL);
		return 0;
	}
	if (logging) {
		fprintf(logFile, "[main] External Version List Initialized\n");
		fflush(logFile);
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
		fflush(logFile);
	}
	
	bool finished = false;
    while(appletMainLoop()) {	
		if (!finished) {
			checkForUpdates(updFile, localVerList, extVerList);
			if (logging) {
				fprintf(logFile, "[main] Finished checkForUpdates\n");
				fflush(logFile);
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
				fflush(logFile);
			}
			
			printf("\nResults printed into sdmc:/Available-Updates.txt!\n\n");
			printf("Press (+) to exit.\n");
			
			if (logging) {
				printf("\nLogging was enabled");
				fflush(logFile);
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
		fflush(logFile);
	}
	freeList(extVerList);
	if (logging) {
		fprintf(logFile, "[main] Freed extVerList\n");
		fflush(logFile);
	}
	fclose(logFile);
	consoleExit(NULL);
    return 0;
}