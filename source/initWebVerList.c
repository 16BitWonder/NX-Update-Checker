#include <curl/curl.h>
#include "main.h"

struct MemoryStruct {
	char *memory;
	size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if(ptr == NULL) {
		/* out of memory! */ 
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

static int xferinfo(
	void *curl,
	curl_off_t dltotal,
	curl_off_t dlnow,
	curl_off_t ultotal,
	curl_off_t ulnow
)
{
	static curl_off_t then = -500;
	curl_off_t now = 0;
	curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME_T, &now);

	if(now - then >= 250 * 1000){
		consoleClear();
		printf("Downloading versions.txt...\n");
		printf("DOWN: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T"\r\n", dlnow, dltotal);
		consoleUpdate(NULL);
		then = now;
	}

	return 0;
}

Entry* initWebVerList()
{
	CURLcode ret = curl_global_init(CURL_GLOBAL_ALL);
	if(ret == 2){
		printf("\nThis application was probably built with a newer libcurl.\n");
		printf("Connections with blanked cal0 will fail.\n");
		printf("Please downgrade switch-curl to 7.69.1-1 and rebuild\n\n");
		printf("Falling back to external downloaded file.\n");
		consoleUpdate(NULL);
		return NULL;
	}
	
	CURL *curl_handle = curl_easy_init();
	
	struct MemoryStruct chunk;
	chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
	chunk.size = 0;    /* no data at this point */

	curl_easy_setopt(curl_handle, CURLOPT_URL, VERSIONS_URL);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
	curl_easy_setopt(curl_handle, CURLOPT_XFERINFOFUNCTION, xferinfo);
	curl_easy_setopt(curl_handle, CURLOPT_XFERINFODATA, curl_handle);
	curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);

	ret = curl_easy_perform(curl_handle);

	curl_easy_cleanup(curl_handle);
	curl_handle = NULL;
	curl_global_cleanup();

	if(ret != CURLE_OK){
		printf("\nFailed to download a copy of versions.txt from web source:\n");
		printf("%s\n\n", VERSIONS_URL);
		printf("Falling back to external downloaded file.\n");
		free(chunk.memory);
		chunk.memory = NULL;
		return NULL;
	} else {
		printf("\nVersion list downloaded with %lu bytes.\n", (unsigned long)chunk.size);
		consoleUpdate(NULL);
	}

	Entry *currExtEntry;
	
	/* Have an empty entry at head of list */
	currExtEntry = calloc(1, sizeof(Entry));	

	return handleVerList(currExtEntry, chunk.size, chunk.memory);
}
