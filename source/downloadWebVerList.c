/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2020, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/ 
/* <DESC>
 * Download a given URL into a local file named page.out.
 * </DESC>
 * Original Source: https://curl.se/libcurl/c/url2file.html
 */ 
 
#include "main.h"
#include <curl/curl.h>

extern bool logging;
extern FILE *logFile;
extern PadState mainPad;
 
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}
 
bool downloadWebVerList() {
	
	printf("Downloading versions.txt...\n");
	consoleUpdate(NULL);
	
	socketInitializeDefault();
	if (logging) {
		fprintf(logFile, "[downloadWebVerList] socketInit finished\n");
		fflush(logFile);
	}
	
	CURL *curl_handle;
	static const char *pagefilename = "tmp";
	FILE *pagefile;
 
	CURLcode ret = curl_global_init(CURL_GLOBAL_ALL);
	if (ret != CURLE_OK) {
		const char *errStr = curl_easy_strerror(ret);
		consoleClear();
		printf("\nFailed to init curl correctly.\n");
		printf("[%d] %s\n\n", ret, errStr);
		printf("Press (+) to fallback to local versions.txt");
		consoleUpdate(NULL);
		
		if (logging) {
			fprintf(logFile, "[downloadWebVerList] Failed to init curl correctly.\n");
			fprintf(logFile, "[downloadWebVerList] [%d] %s\n", ret, errStr);
			fflush(logFile);
		}
		
		while (1) {
			padUpdate(&mainPad);
			u64 kDown = padGetButtonsDown(&mainPad);
			if (kDown & HidNpadButton_Plus) {
				curl_global_cleanup();
				socketExit();
				consoleClear();
				return false;
			}
		}
	}
	if (logging) {
		fprintf(logFile, "[downloadWebVerList] curl_global_init finished\n");
		fflush(logFile);
	}
 
	/* init the curl session */ 
	curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
	if (logging) {
		fprintf(logFile, "[downloadWebVerList] curl_easy_init finished\n");
		fflush(logFile);
	}
 
	/* set URL to get here */ 
	curl_easy_setopt(curl_handle, CURLOPT_URL, VERSIONS_URL);
	if (logging) {
		fprintf(logFile, "[downloadWebVerList] 1st curl_easy_setopt finished\n");
		fflush(logFile);
	}
 
	/* disable progress meter, set to 0L to enable it */ 
	curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
	if (logging) {
		fprintf(logFile, "[downloadWebVerList] 2nd curl_easy_setopt finished\n");
		fflush(logFile);
	}
 
	/* send all data to this function	*/ 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
	if (logging) {
		fprintf(logFile, "[downloadWebVerList] 3rd curl_easy_setopt finished\n");
		fflush(logFile);
	}
 
	/* open the file */ 
	pagefile = fopen(pagefilename, "wb");
	if(pagefile) {
 
		/* write the page body to this file handle */ 
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);
		if (logging) {
			fprintf(logFile, "[downloadWebVerList] 4th curl_easy_setopt finished\n");
			fflush(logFile);
		}
 
		/* get it! */ 
		ret = curl_easy_perform(curl_handle);
		if (logging) {
			fprintf(logFile, "[downloadWebVerList] curl_easy_perform finished\n");
			fflush(logFile);
		}
		
		/* close the header file */ 
		fclose(pagefile);
		if (logging) {
			fprintf(logFile, "[downloadWebVerList] fclose finished\n");
			fflush(logFile);
		}
		
		if (ret != CURLE_OK) {
			const char *errStr = curl_easy_strerror(ret);
			consoleClear();
			printf("\nFailed to download versions.txt from web source:\n");
			printf("%s\n\n", VERSIONS_URL);
			printf("[%d] %s\n\n", ret, errStr);
			printf("Press (+) to fallback to local versions.txt");
			consoleUpdate(NULL);
		
			if (logging) {
				fprintf(logFile, "[downloadWebVerList] Failed to download versions.txt from web source:\n");
				fprintf(logFile, "[downloadWebVerList] %s\n", VERSIONS_URL);
				fprintf(logFile, "[downloadWebVerList] [%d] %s\n", ret, errStr);
				fflush(logFile);
			}
		
			while (1) {
				padUpdate(&mainPad);
				u64 kDown = padGetButtonsDown(&mainPad);
				if (kDown & HidNpadButton_Plus) {
					remove(pagefilename);
					curl_easy_cleanup(curl_handle);
					curl_global_cleanup();
					socketExit();
					consoleClear();
					return false;
				}
			}
		}
	}
 
	/* cleanup curl stuff */ 
	curl_easy_cleanup(curl_handle);
	if (logging) {
		fprintf(logFile, "[downloadWebVerList] curl_easy_cleanup finished\n");
		fflush(logFile);
	}
 
	curl_global_cleanup();
	if (logging) {
		fprintf(logFile, "[downloadWebVerList] curl_global_cleanup finished\n");
		fflush(logFile);
	}
	
	socketExit();
	if (logging) {
		fprintf(logFile, "[downloadWebVerList] socketExit finished\n");
		fflush(logFile);
	}
	
	/* Be certain our file actually downloaded */
	if(access("tmp", F_OK ) == -1 ) {
		if (logging) {
			fprintf(logFile, "[downloadWebVerList] versions.txt not downloaded\n");
			fflush(logFile);
		}
		consoleClear();
		return false;
	}
	
	/* Replace versions.txt with tmp */
	remove("versions.txt");
	if (logging) {
		fprintf(logFile, "[downloadWebVerList] Old versions.txt removed\n");
		fflush(logFile);
	}
	rename(pagefilename, "versions.txt");
	if (logging) {
		fprintf(logFile, "[downloadWebVerList] tmp renamed to versions.txt\n");
		fflush(logFile);
	}
 
	consoleClear();
	return true;
}