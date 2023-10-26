#include "ns.hpp"
#include <fstream>
#include <cmath>

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
		
		// Find versions.txt
		std::ifstream extFile(filepath);
		if (!extFile.good()) {
			// File not found or inaccessible, prompt them to update external list
		}
		
		return extVersions;
	}
}