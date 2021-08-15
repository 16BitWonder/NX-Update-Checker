#include "main.h"

extern PadState pad;

int getGCDBCount(char* index);
bool gameCardHasBundledUpdate();
Result getGameCardApplicationIDAndVersion(u64 *application_id, s32 *version);
Result getGameCardMetaKey(NcmApplicationContentMetaKey *metaKey, NcmContentMetaType metaType);
char* readDBFileIntoMem();

void updateStoredCartVerList() {
	
	// Initialize services
	ncmInitialize();
	nsInitialize();
	
	// Check if a game card is inserted
	bool cardInserted = false;
	nsIsGameCardInserted(&cardInserted);
	
	// Return if no game card inserted
	if (!cardInserted) {
		nsExit();
		ncmExit();
		return;
	}
	
	u64 tid = 0;
	s32 version = 0;
	
	// Get game card title ID and version
	getGameCardApplicationIDAndVersion(&tid, &version);
	
	// Change tid to its patch ID
	tid += 2048;
	
	// Read the file into a char*
	char *gcDBMem = readDBFileIntoMem();
	
	// Count the number of entries and then malloc the required Cart_Entry size
	int currDBEntries = getGCDBCount(gcDBMem);
	
	// Copy each entry into the malloc'd *Cart_Entry
	if (currDBEntries != 0) {
		
		// Allocate space for the entries (plus one empty in case we need to add a new entry)
		CartEntry Cart_Entry[currDBEntries+1];
		
		char *gcDBMemIndex = gcDBMem;
		
		// Fill out the entries
		char tmpTid[17];
		tmpTid[16] = '\0';
		for (int i = 0; i < currDBEntries; i++) {
			
			// Get the next title ID
			strncpy(tmpTid, gcDBMemIndex, 16);
			Cart_Entry[i].patch_id = strtol(tmpTid, NULL, 16);
			gcDBMemIndex += 17;
			
			// Get the next version
			Cart_Entry[i].version = 0;
			while (*gcDBMemIndex != '\n') {
				Cart_Entry[i].version *= 10;
				Cart_Entry[i].version += *gcDBMemIndex - '0';
				gcDBMemIndex++;
			}
			gcDBMemIndex++;
		}
		free(gcDBMem);
	
		// Run through current db and update if a newer version has been found
		bool updatedEntry = false;
		bool noChange = false;
		for (int i = 0; i < currDBEntries; i++) {
		
			if (tid == Cart_Entry[i].patch_id) {
				if (version > Cart_Entry[i].version) {
					Cart_Entry[i].version = version;
					i = currDBEntries;
					updatedEntry = true;
				} else {
					noChange = true;
				}
			}
		}
	
		// If we didn't update an already present entry, then this is a new one that should be appended
		if (!noChange && !updatedEntry) {
			
			// Bump currDBEntries
			currDBEntries++;
			
			// Populate appended entry
			Cart_Entry[currDBEntries-1].patch_id = tid;
			Cart_Entry[currDBEntries-1].version = version;
		}
		
		// Write new cart_versions.txt, overwriting any old one if changes were made
		if (!noChange){
			// Open cart_versions.txt
			FILE *gcDB = fopen("cart_versions.txt", "w");
			for (int i = 0; i < currDBEntries; i++) {
			
				fprintf(gcDB, "%.16lX|%d\n", Cart_Entry[i].patch_id, Cart_Entry[i].version);
			}
			fclose(gcDB);
		}
	} else {
		
		// Open cart_versions.txt
		FILE *gcDB = fopen("cart_versions.txt", "w");
		
		// Print found cartridge info
		fprintf(gcDB, "%.16lX|%d\n", tid, version);
		
		fclose(gcDB);
	}
	
	ncmExit();
	nsExit();
	return;
}

// Reads cart_versions.txt into a char* and returns a pointer to it
char* readDBFileIntoMem() {
	
	char *fileRead;
	
	// Open cart_versions.txt
	FILE *gcDB = fopen("cart_versions.txt", "r");
	
	// If cart_versions does not exist, return NULL
	if (gcDB == NULL) return NULL;
		
	// Read the file into a char*
	fseek(gcDB, 0L, SEEK_END);
	long fileSize = ftell(gcDB);
	fileRead = calloc(fileSize, sizeof(char));
	rewind(gcDB);
	fread(fileRead, 1, fileSize, gcDB);
	fclose(gcDB);
	
	return fileRead;
}

// Gets the count of entries in cart_versions.txt if it exists, 0 otherwise
int getGCDBCount(char *index) {
	
	int entries = 0;
	
	// If we have no file to read return
	if (index == NULL) return entries;
	
	while (*index != '\0') {
		if (*index == '\n') entries++;
		index++;
	}
	
	return entries;
}

// Returns true if a game card has a bundled update, false otherwise
bool gameCardHasBundledUpdate() {
	
	NcmContentMetaDatabase db;
	NcmApplicationContentMetaKey tmpMetaKey;
	s32 total = 0, written = 0;
	ncmOpenContentMetaDatabase(&db, NcmStorageId_GameCard);
	
	// Grab the application meta key
	ncmContentMetaDatabaseListApplication(&db, &total, &written, &tmpMetaKey, 1, NcmContentMetaType_Patch);
	
	// Return true if an update was found
	return total != 0;
}

// Returns the game cards version number and base title ID
Result getGameCardApplicationIDAndVersion(u64 *application_id, s32 *version) {
	
	NcmApplicationContentMetaKey baseMetaKey;
	Result rc_baseMetaKey = getGameCardMetaKey(&baseMetaKey, NcmContentMetaType_Application);
	
	if (gameCardHasBundledUpdate()) {
		
		NcmApplicationContentMetaKey updateMetaKey;
		Result rc_updateMetaKey = getGameCardMetaKey(&updateMetaKey, NcmContentMetaType_Patch);
		
		// Stop if we failed to retrieve either key
		if (R_FAILED(rc_baseMetaKey) || R_FAILED(rc_updateMetaKey)) return -1;
		
		// Figure out the largest version number between base/update and return it
		s32 tmpVersion = updateMetaKey.key.version;
		if (tmpVersion < baseMetaKey.key.version) tmpVersion = baseMetaKey.key.version;
		*version = tmpVersion;
	} else {
		
		// Stop if we failed to retrieve base key
		if (R_FAILED(rc_baseMetaKey)) return -1;
		
		// Return version
		*version = baseMetaKey.key.version;
	}
	
	// Return title ID
	*application_id = baseMetaKey.application_id;
	
	return 0;
}

// Returns a NcmApplicationContentMetaKey for the defined NcmContentMetaType
Result getGameCardMetaKey(NcmApplicationContentMetaKey *metaKey, NcmContentMetaType metaType) {
	
	NcmContentMetaDatabase db;
	NcmApplicationContentMetaKey tmpMetaKey;
	s32 total = 0, written = 0;
	Result rc = ncmOpenContentMetaDatabase(&db, NcmStorageId_GameCard);
	
	// Grab the application meta key
	rc = ncmContentMetaDatabaseListApplication(&db, &total, &written, &tmpMetaKey, 1, metaType);
	
	// Return meta key
	*metaKey = tmpMetaKey;
	
	ncmContentMetaDatabaseClose(&db);
	return rc;
}