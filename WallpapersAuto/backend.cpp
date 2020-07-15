#include "backend.h"

// Function where main computations take place
// Need to structure calculations
void backend() {	
	urlGen();							// generate URL's
	int numberOfJsonFiles = getJson();	// get json files from those URL's
	getDataFromJson(numberOfJsonFiles);	// get wallpapers from json files
}
