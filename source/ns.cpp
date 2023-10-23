#include "ns.hpp"
#include <fstream>

namespace ns {
	VersionDB initExternalVersions() {
		VersionDB extVersions;
		
		// Find versions.txt
		std::ifstream extFile("versions.txt");
		if (!extFile.good()) {
			// File not found or inaccessible, prompt them to update external list
		}
		
		return extVersions;
	}
}