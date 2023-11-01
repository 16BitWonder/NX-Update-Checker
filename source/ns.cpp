#include "ns.hpp"
#include <fstream>
#include <cmath>

extern Controller controller;

namespace ns {
	
	// Takes a hex-char and index and returns it as a decimal number
	char hexCharToNum(char hexChar, int index) {
		std::string hexNumbers = "0123456789";
		std::string hexLetters = "ABCDEF";
		
		if (hexNumbers.find(hexChar) != std::string::npos) {
			hexChar -= 48;
		} else if (hexLetters.find(hexChar) != std::string::npos) {
			hexChar -= 55;
		}
		return hexChar;
	}
	
	// Takes a 16-character application ID string and returns it as a u64
	u64 applicationIDStringToU64(std::string applicationIDString) {
		
		u64 application_id = 0;
		
		for (int i = 0; i < 16; i++) {
			char currVal = hexCharToNum(applicationIDString.at(i), i);
			application_id += currVal * (pow(16, 15-i));
		} 
		return application_id;
	}
	
	// Takes a read version entry then updates title, application_id and version with the parsed info
	void parseApplicationIDAndVersion(std::string readLine, std::string* title, u64* application_id, u32* version, bool parseTitle = false) {
		const char delimiter = '|';
		
		// Parse the title, used for cart_versions.txt, update readLine by dropping title
		std::string readTitle = "";
		if (parseTitle) {
			readTitle = readLine.substr(0, readLine.find(delimiter));
			readLine = readLine.substr(readLine.find(delimiter)+1, std::string::npos);
		}
		*title = readTitle;
		
		// Parse application ID and update readLine, dropping appID
		std::string appIDString = readLine.substr(0, readLine.find(delimiter));
		readLine = readLine.substr(readLine.find(delimiter)+1, std::string::npos);
		*application_id = applicationIDStringToU64(appIDString);
		
		// Parse version
		*version = atoi(readLine.c_str());
		
		return;
	}
	
	// Takes a file path to a versions text file, builds a VersionDB Object and returns it
	VersionDB initExternalVersions(std::string filepath) {
		VersionDB extVersions;
		
		// Find input file
		std::ifstream extFile(filepath, std::ifstream::in);
		if (!extFile.good()) {
			// Not found or not opened
			printf("%s could not be opened\n", filepath.c_str());
			printf("Press B to return\n");
			consoleUpdate(NULL);
			while (true) {
				if (controller.getNewPressed() & HidNpadButton_B) return extVersions;
			}
		}
		
		// Determine if it was a version list with titles
		bool hasTitles = filepath != "versions.txt";
		
		// TODO - Should add a loading bar here
		
		// Read through file until EOF, read/toss the header line before loop
		std::string currLine = "";
		getline(extFile, currLine);
		while (!extFile.eof()) {
			
			// Get next line from file
			getline(extFile, currLine);
			
			// Don't execute the rest of the loop if we've reached EOF
			if (!extFile.eof()) {
				// Parse info from this line
				std::string title = "";
				u64 application_id = 0;
				u32 version = 0;
				parseApplicationIDAndVersion(currLine, &title, &application_id, &version, hasTitles);
				
				// Add parsed info to extVersions
				extVersions.addTitleStrings(application_id, version, title);
			}
		}
		
		return extVersions;
	}
}