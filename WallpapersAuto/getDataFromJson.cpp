#include "getDataFromJson.h"

// Check that file exist
bool fileExists(std::string fname) { 
	std::ifstream in(fname.c_str());
	if (in.is_open()) {
		in.close();
		return true;
	}
	else {
		in.close();
		return false;
	}
}

// Get wallpapers from json
void getDataFromJson(int numberOfURLs) {
	rapidjson::Document doc;
	FILE* jsonFile;
	FILE* wallpaperFile;
	FILE* getAmount;
	CURL* curl = NULL; curl = curl_easy_init();
	CURLcode err;

	// Get path for saving wallpapers
	std::ifstream getPath("tempFiles/path.txt");
	std::string path;
	getPath >> path;
	getPath.close();

	// Get amount of wallpapers
	int amountOfWallpapers;
	getAmount = fopen("tempFiles/result.txt", "r");
	fscanf(getAmount, "%*d%*d%*s%d", &amountOfWallpapers);
	fclose(getAmount);

	// Random distribution between tags
	std::vector <int> a(numberOfURLs, 0);
	srand(time(NULL));
	int sumOfRandomDistribution = amountOfWallpapers;
	for (int i = 0; (i < numberOfURLs - 1) && (sumOfRandomDistribution > 0); ++i) {
		a[i] = 1 + rand() % sumOfRandomDistribution;
		sumOfRandomDistribution -= a[i];
	}
	a[numberOfURLs - 1] = sumOfRandomDistribution;

	// Get wallpapers with every tag
	std::string jsonPath = "", url = "", wallpaperPath = "";
	for (int i = 1; i <= numberOfURLs; ++i) { // iterate over tags
		// Parse json file
		jsonPath = "tempFiles/" + std::to_string(i) + ".txt";
		jsonFile = fopen(jsonPath.c_str(), "rb");
		char buffer[65536];
		rapidjson::FileReadStream is(jsonFile, buffer,  sizeof(buffer));
		doc.ParseStream(is);
		if (doc.HasParseError()) {
			std::cerr << "Parse Error!" << std::endl;
		}
		if (doc["data"].Empty()) { // if it finds nothing for this tag
			continue;
		}

		// Get wallpapers 
		int currentNumberOfWallpapers = doc["data"].Size();				
		srand(time(NULL));
		for (int j = 0; j < a[i - 1]; ++j) { // iterates over the amount of wallpapers
			// Generate path for current wallpaper
			wallpaperPath = path + "\\" + std::to_string(rand()) + ".jpg";
			while (fileExists(wallpaperPath)) {
				wallpaperPath = path + "\\" + std::to_string(rand()) + ".jpg";
			}
			wallpaperFile = fopen(wallpaperPath.c_str(), "wb");

			// Get wallpaper
			url = doc["data"][rand() % currentNumberOfWallpapers]["path"].GetString();
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, wallpaperFile);
			err = curl_easy_perform(curl);
			if (err != CURLE_OK) {
				std::cerr << "Curl_easy_setopt error! \n" << curl_easy_strerror(err) << std::endl;
			}
			curl_easy_reset(curl);
			fclose(wallpaperFile);
		}
		fclose(jsonFile);
	}
	curl_easy_cleanup(curl);
}
