#include "main.h"

extern bool logging;
extern FILE *logFile;
Entry *ignoreList;

void printNewUpdates(FILE*, Entry*, Entry*);
void printNewDLC(FILE*, Entry*, Entry*);
void printMissDLC(FILE*, Entry*, Entry*);
void initIgnoreList();
bool isIgnored(char*);

void checkForUpdates(FILE *out, Entry *localEntries, Entry *extEntries) {

	u64 rawTime;
	TimeCalendarTime timeCal;
	TimeCalendarAdditionalInfo timeInfo;
	timeGetCurrentTime(TimeType_UserSystemClock, &rawTime);
	timeToCalendarTimeWithMyRule(rawTime, &timeCal, &timeInfo);
	
	fprintf(out, "Generated: %04i/%02i/%02i - %02i:%02i:%02i (%s)\n\n\n", timeCal.year, timeCal.month, timeCal.day, timeCal.hour, timeCal.minute, timeCal.second, timeInfo.timezoneName);
	
	if (logging) {
		fprintf(logFile, "[checkForUpdates] Begin initIgnoreList\n");
		fflush(logFile);
	}
	initIgnoreList();
	if (logging) {
		fprintf(logFile, "[checkForUpdates] ignoreList initialized\n");
		fflush(logFile);
	}
	
	if (logging) {
		fprintf(logFile, "[checkForUpdates] Begin printing new updates\n");
		fflush(logFile);
	}
	printNewUpdates(out, localEntries, extEntries);
	if (logging) {
		fprintf(logFile, "[checkForUpdates] Finished printing new updates\n");
		fprintf(logFile, "[checkForUpdates] Begin printing new DLC binaries\n");
		fflush(logFile);
	}
	printNewDLC(out, localEntries, extEntries);
	if (logging) {
		fprintf(logFile, "[checkForUpdates] Finished printing new DLC binaries\n");
		fprintf(logFile, "[checkForUpdates] Begin printing missing DLC\n");
		fflush(logFile);
	}
	printMissDLC(out, localEntries, extEntries);
	if (logging) {
		fprintf(logFile, "[checkForUpdates] Finished printing missing DLC\n");
		fflush(logFile);
	}
	
	freeList(ignoreList);
	if (logging) {
		fprintf(logFile, "[checkForUpdates] Freed ignoreList\n");
		fflush(logFile);
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
			fflush(logFile);
		}
		while (tmpExt != NULL) {
			if (strcmp(tmpExt->Data.TID, tmpLocal->Data.TID) == 0) {
				if ((tmpLocal->Data.TID[13] == '8') && (tmpExt->Data.version > tmpLocal->Data.version)) {
					if (logging) {
						fprintf(logFile, "[printNewUpdates] Found newer update, checking if it's ignored\n");
						fflush(logFile);
					}
					if (!isIgnored(tmpLocal->Data.TID)) {
						if (logging) {
							fprintf(logFile, "[printNewUpdates] Not ignored, printing out\n");
							fflush(logFile);
						}
						fprintf(out, "%s [%s][%s][v%d] -> [v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.displayVersion, tmpLocal->Data.version, tmpExt->Data.version);
						printf("%s [%s][%s][v%d] -> [v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.displayVersion, tmpLocal->Data.version, tmpExt->Data.version);
						if (logging) {
							fprintf(logFile, "[printNewUpdates] Printed %s [%s][%s][v%d] -> [v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.displayVersion, tmpLocal->Data.version, tmpExt->Data.version);
							fflush(logFile);
						}
						consoleUpdate(NULL);
					} else {
						if (logging) {
							fprintf(logFile, "[printNewUpdates] Ignoring %s\n", tmpLocal->Data.TID);
							fflush(logFile);
						}
					}
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
			fflush(logFile);
		}
		while (tmpExt != NULL) {
			if (strcmp(tmpExt->Data.TID, tmpLocal->Data.TID) == 0) {
				if ((tmpLocal->Data.TID[13] != '8') && (tmpExt->Data.version > tmpLocal->Data.version)) {
					if (logging) {
						fprintf(logFile, "[printNewDLC] Found newer DLC, checking if it's ignored\n");
						fflush(logFile);
					}
					if (!isIgnored(tmpLocal->Data.TID)) {
						if (logging) {
							fprintf(logFile, "[printNewDLC] Not ignored, printing out\n");
							fflush(logFile);
						}
						fprintf(out, "%s [%s][v%d] -> [v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.version, tmpExt->Data.version);
						printf("%s [%s][v%d] -> [v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.version, tmpExt->Data.version);
						if (logging) {
							fprintf(logFile, "[printNewDLC] Printed %s [%s][v%d] -> [v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.version, tmpExt->Data.version);
							fflush(logFile);
						}
						consoleUpdate(NULL);
					} else {
						if (logging) {
							fprintf(logFile, "[printNewDLC] Ignoring %s\n", tmpLocal->Data.TID);
							fflush(logFile);
						}
					}
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
			fflush(logFile);
		} else if (logging) {
			fprintf(logFile, "[printMissDLC] Reached the end of the local title list\n");
			fflush(logFile);
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
				if (!foundDLC && !isIgnored(dlcList->Data.TID)) {
					
					fprintf(out, "%s [%s][v%d]\n", localTraverse->Data.name, dlcList->Data.TID, dlcList->Data.version);
					printf("%s [%s][v%d]\n", localTraverse->Data.name, dlcList->Data.TID, dlcList->Data.version);
					if (logging) {
						fprintf(logFile, "[printMissDLC] Printed %s [%s][v%d]\n", localTraverse->Data.name, dlcList->Data.TID, dlcList->Data.version);
						fflush(logFile);
					}
					consoleUpdate(NULL);
				} else if (isIgnored(dlcList->Data.TID)) {
					if (logging) {
						fprintf(logFile, "[printMissDLC] Ignoring %s\n", dlcList->Data.TID);
						fflush(logFile);
					}
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

void initIgnoreList() {
	// Have an empty entry at head of list
	ignoreList = calloc(1, sizeof(Entry));
	
	if(access("ignore.txt", F_OK ) == -1 ) {
		if (logging) {
			fprintf(logFile, "[initIgnoreList] ignore.txt does not exist\n");
			fflush(logFile);
		}
		return;
	}
	
	Entry *tmpIgnore = ignoreList;
	
	char *memVerList;
	FILE *ignoreListTxt = fopen("ignore.txt", "r");
	if (logging) {
		fprintf(logFile, "[initIgnoreList] ignore.txt opened\n");
		fflush(logFile);
	}
	
	fseek(ignoreListTxt, 0, SEEK_END);
	long fileSize = ftell(ignoreListTxt);
	memVerList = calloc(fileSize, sizeof(char));
	rewind(ignoreListTxt);
	fread(memVerList, 1, fileSize, ignoreListTxt);
	if (logging) {
		fprintf(logFile, "[initIgnoreList] Read ignore.txt into memory\n");
		fflush(logFile);
	}
	
	// Close our input file
	if (fclose(ignoreListTxt) != 0) {
		perror("ignore.txt");
		consoleUpdate(NULL);
	}
	if (logging) {
		fprintf(logFile, "[initIgnoreList] ignore.txt closed\n");
		fflush(logFile);
	}
	
	// Build ignore list
	char titleID[17];
	titleID[16] = 0;
	long fileIndex = 0;
	int tidIndex = 0;
	while (fileIndex < fileSize) {
		titleID[tidIndex] = *(memVerList+fileIndex);
		if (tidIndex == 15) {
			tidIndex = -1;
			fileIndex += 2;
			Entry *newIgnore = calloc(1, sizeof(Entry));
			strcpy(newIgnore->Data.TID, titleID);
			tmpIgnore->next = newIgnore;
			newIgnore->prev = tmpIgnore;
			tmpIgnore = tmpIgnore->next;
			if (logging) {
				fprintf(logFile, "[initIgnoreList] Parsed %s into ignore list\n", titleID);
				fflush(logFile);
			}
		}
		fileIndex++;
		tidIndex++;
	}
	
	return;
}

bool isIgnored(char *titleID) {
	Entry *tmpEntry = ignoreList->next;
	
	while (tmpEntry != NULL) {
		if (strcmp(titleID, tmpEntry->Data.TID) == 0) {
			return true;
		}
		tmpEntry = tmpEntry->next;
	}
	
	return false;
}