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
	
	fprintf(out, "Generated: %04i/%02i/%02i - %02i:%02i:%02i (%s)\n", timeCal.year, timeCal.month, timeCal.day, timeCal.hour, timeCal.minute, timeCal.second, timeInfo.timezoneName);
	
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
	//printMissDLC(out, localEntries, extEntries);
	if (logging) {
		fprintf(logFile, "[checkForUpdates] Finish printing missing DLC\n");
	}
	
    return;
}

void printNewUpdates(FILE *out, Entry *localEntries, Entry *extEntries) {
	Entry *tmpLocal = localEntries->next;
	Entry *tmpExt;
	
	fprintf(out, "New Updates Available:\n\n");
	
	while (tmpLocal != NULL) {
		tmpExt = extEntries->next;
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
	
	while (tmpLocal != NULL) {
		tmpExt = extEntries->next;
		while (tmpExt != NULL) {
			if (strcmp(tmpExt->Data.TID, tmpLocal->Data.TID) == 0) {
				if ((tmpLocal->Data.TID[13] == '8') && (tmpExt->Data.version > tmpLocal->Data.version)) {
					fprintf(out, "%s [%s][%s][v%d] -> [v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.displayVersion, tmpLocal->Data.version, tmpExt->Data.version);
					printf("%s [%s][%s][v%d] -> [v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.displayVersion, tmpLocal->Data.version, tmpExt->Data.version);
					if (logging) {
						fprintf(logFile, "[printNewDLC] Printed %s [%s][%s][v%d] -> [v%d]\n", tmpLocal->Data.name, tmpLocal->Data.TID, tmpLocal->Data.displayVersion, tmpLocal->Data.version, tmpExt->Data.version);
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
	
	fprintf(out, "Available DLC Not Installed:\n\n");
	
	// TODO
	
	fprintf(out, "\n\n");
	printf("\n\n");
	return;
}