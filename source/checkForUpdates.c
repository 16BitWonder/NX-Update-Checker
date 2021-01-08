#include "main.h"

extern bool logging;
extern FILE *logFile;

void printNewUpdates(FILE*, Entry*, Entry*);
void printNewDLC(FILE*, Entry*, Entry*);
void printMissDLC(FILE*, Entry*, Entry*);

void checkForUpdates(FILE *out, Entry *localEntries, Entry *extEntries) {

	u64 rawTime;
	TimeCalendarTime timeCal;
	TimeCalendarAdditionalInfo timeInfo;
	timeGetCurrentTime(TimeType_UserSystemClock, &rawTime);
	timeToCalendarTimeWithMyRule(rawTime, &timeCal, &timeInfo);
	
	fprintf(out, "Generated: %04i/%02i/%02i - %02i:%02i:%02i (%s)\n\n\n", timeCal.year, timeCal.month, timeCal.day, timeCal.hour, timeCal.minute, timeCal.second, timeInfo.timezoneName);
	
	if (logging) {
		fprintf(logFile, "[checkForUpdates] Begin printing new updates\n");
	}
	printNewUpdates(out, localEntries, extEntries);
	if (logging) {
		fprintf(logFile, "[checkForUpdates] Finished printing new updates\n");
		fprintf(logFile, "[checkForUpdates] Begin printing new DLC binaries\n");
	}
	printNewDLC(out, localEntries, extEntries);
	if (logging) {
		fprintf(logFile, "[checkForUpdates] Finished printing new DLC binaries\n");
		fprintf(logFile, "[checkForUpdates] Begin printing missing DLC\n");
	}
	printMissDLC(out, localEntries, extEntries);
	if (logging) {
		fprintf(logFile, "[checkForUpdates] Finished printing missing DLC\n");
	}
	
    return;
}

void printNewUpdates(FILE *out, Entry *localEntries, Entry *extEntries) {
	Entry *tmpLocal = localEntries->next;
	Entry *tmpExt;
	
	fprintf(out, "New Updates Available:\n\n");
	printf("New Updates Available:\n\n");
	
	while (tmpLocal != NULL) {
		tmpExt = extEntries->next;
		if ((tmpLocal->Data.TID[13] == '8') && logging) {
			fprintf(logFile, "[printNewUpdates] Looking for newer update of %s [%s][%s][v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.displayVersion, tmpLocal->Data.version);
		}
		while (tmpExt != NULL) {
			if (strcmp(tmpExt->Data.TID, tmpLocal->Data.TID) == 0) {
				if ((tmpLocal->Data.TID[13] == '8') && (tmpExt->Data.version > tmpLocal->Data.version)) {
					fprintf(out, "%s [%s][%s][v%d] -> [v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.displayVersion, tmpLocal->Data.version, tmpExt->Data.version);
					printf("%s [%s][%s][v%d] -> [v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.displayVersion, tmpLocal->Data.version, tmpExt->Data.version);
					if (logging) {
						fprintf(logFile, "[printNewUpdates] Printed %s [%s][%s][v%d] -> [v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.displayVersion, tmpLocal->Data.version, tmpExt->Data.version);
					}
					consoleUpdate(NULL);
				}
				tmpExt = NULL;
			} else {
				tmpExt = tmpExt->next;
			}
		}
		tmpLocal = tmpLocal->next;
	}
	
	fprintf(out, "\n\n");
	printf("\n\n");
	return;
}

void printNewDLC(FILE *out, Entry *localEntries, Entry *extEntries) {
	Entry *tmpLocal = localEntries->next;
	Entry *tmpExt;
	
	fprintf(out, "New DLC Binaries Available:\n\n");
	printf("New DLC Binaries Available:\n\n");
	
	while (tmpLocal != NULL) {
		tmpExt = extEntries->next;
		if ((tmpLocal->Data.TID[13] != '8') && logging) {
			fprintf(logFile, "[printNewDLC] Looking for newer DLC of %s [%s][v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.version);
		}
		while (tmpExt != NULL) {
			if (strcmp(tmpExt->Data.TID, tmpLocal->Data.TID) == 0) {
				if ((tmpLocal->Data.TID[13] != '8') && (tmpExt->Data.version > tmpLocal->Data.version)) {
					fprintf(out, "%s [%s][v%d] -> [v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.version, tmpExt->Data.version);
					printf("%s [%s][v%d] -> [v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.version, tmpExt->Data.version);
					if (logging) {
						fprintf(logFile, "[printNewDLC] Printed %s [%s][v%d] -> [v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.version, tmpExt->Data.version);
					}
					consoleUpdate(NULL);
				}
				tmpExt = NULL;
			} else {
				tmpExt = tmpExt->next;
			}
		}
		tmpLocal = tmpLocal->next;
	}
	
	fprintf(out, "\n\n");
	printf("\n\n");
	return;
}

void printMissDLC(FILE *out, Entry *localEntries, Entry *extEntries) {
	Entry *localTraverse = localEntries->next;
	Entry *tmpLocal;
	Entry *tmpExt;
	Entry *dlcList = calloc(1, sizeof(Entry));
	Entry *dlcListHead = dlcList;
	char dlcRange[17];
	
	fprintf(out, "Available DLC Not Installed:\n\n");
	printf("Available DLC Not Installed:\n\n");
	
	while (localTraverse != NULL) {
		// Find title TID
		while ((localTraverse != NULL) && (localTraverse->Data.TID[13] != '8')) {
			localTraverse = localTraverse->next;
		}
		if ((localTraverse != NULL) && logging) {
			fprintf(logFile, "[printMissDLC] Looking for missing DLC for %s\n", localTraverse->Data.TID);
		} else if (logging) {
			fprintf(logFile, "[printMissDLC] Reached the end of the local title list\n");
		}
	
		if (localTraverse != NULL) {
			// Shift TID into DLC range
			strcpy(dlcRange, localTraverse->Data.TID);
			dlcRange[12] = dlcRange[12] + 1;
			if (dlcRange[12] == ':'){
				dlcRange[12] = 'A';
			}
		
			// Iterate through external list looking for DLC
			tmpExt = extEntries->next;
			while (tmpExt != NULL) {
				strcpy(dlcRange+13, (tmpExt->Data.TID)+13);
				if (strcmp(dlcRange, tmpExt->Data.TID) == 0) {
					Entry *newDLC = calloc(1, sizeof(Entry));
					strcpy(newDLC->Data.TID, tmpExt->Data.TID);
					newDLC->Data.version = tmpExt->Data.version;
					newDLC->prev = dlcList;
					dlcList->next = newDLC;
					dlcList = dlcList->next;
				}
				tmpExt = tmpExt->next;
			}
	
			// Check if any of this title's DLC are not installed
			dlcList = dlcListHead->next;
			while (dlcList != NULL) {
				bool foundDLC = false;
				tmpLocal = localEntries->next;
				// If local titles don't include this DLC, print it
				while (!foundDLC && (tmpLocal != NULL)) {
					if (strcmp(dlcList->Data.TID, tmpLocal->Data.TID) == 0) {
						foundDLC = true;
					}
					tmpLocal = tmpLocal->next;
				}
				if (!foundDLC) {
					fprintf(out, "%s [%s][v%d]\n", localTraverse->Data.name, dlcList->Data.TID, dlcList->Data.version);
					printf("%s [%s][v%d]\n", localTraverse->Data.name, dlcList->Data.TID, dlcList->Data.version);
					if (logging) {
						fprintf(logFile, "[printMissDLC] Printed %s [%s][v%d]\n", localTraverse->Data.name, dlcList->Data.TID, dlcList->Data.version);
					}
					consoleUpdate(NULL);
				}
				dlcList = dlcList->next;
			}
			freeList(dlcListHead);
			dlcListHead = calloc(1, sizeof(Entry));
			dlcList = dlcListHead;
			localTraverse = localTraverse->next;
		}
	}

	return;
}