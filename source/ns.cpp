#include "ns.hpp"
#include <fstream>

namespace ns {
	
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