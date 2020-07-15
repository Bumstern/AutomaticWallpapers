#include "dataFromUrl.h"

// URL generation 
void urlGen() {
    // Get URL's parameters 
    std::ifstream inResult("tempFiles/result.txt");
    int random;
    std::string ratio, purity;
    inResult >> random >> purity >> ratio;
    inResult.close();

    // Tags input
    std::vector<std::string> tags;
    if (random == 0) {
        std::ifstream inTags("tempFiles/tag.txt");
        std::string temp;
        while (std::getline(inTags, temp)) {
            if (temp == "\n") {
                continue;
            }
            tags.push_back(temp);
        }
        inTags.close();
    }

    // API input 
    std::ifstream inApi("tempFiles/api.txt");
    std::string api;
    inApi >> api;
    inApi.close();

    // URL generation
    std::ofstream outUrl("tempFiles/url.txt");
    if (random == 0) {  // generation with tags
        outUrl << tags.size() << '\n';
        for (int i = 0; i < tags.size(); ++i) {
            outUrl << "https://wallhaven.cc/api/v1/search?q=" << tags[i] << "&apikey=" << api << "&purity=" << purity << "&ratios=" << ratio << '\n';
        }
    }
    else {  // random generation
        outUrl << 1 << '\n';
        outUrl << "https://wallhaven.cc/api/v1/search?apikey=" << api << "&sorting=random&categories=111&purity=" << purity << "&ratios=" << ratio;
    }
    outUrl.close();
} 

// Get Json files from URL's
int getJson() {
    CURL* curl; curl = curl_easy_init();
    CURLcode err;
    FILE* apiFile;

    std::ifstream getURL("tempFiles/url.txt");
    int numberOfURLs; getURL >> numberOfURLs; getURL.ignore(1);
    std::string url, apiFilePath = "";
    for (int i = 1; i <= numberOfURLs; ++i) {
        // Getting new url from file
        getline(getURL, url);

        // Create path where we will save json file
        apiFilePath = "tempFiles/" + std::to_string(i) + ".txt";

        // Get json file of first page
        apiFile = fopen(apiFilePath.c_str(), "w");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, apiFile);
        err = curl_easy_perform(curl);
        if (err != CURLE_OK) {
            std::cerr << "Curl_easy_setopt error! \n" << curl_easy_strerror(err) << std::endl;
        }
        curl_easy_reset(curl);
        fclose(apiFile);

        // Take max number of pages from this json file
        // and than we use it to get json file of random page
        char buffer[65536];
        apiFile = fopen(apiFilePath.c_str(), "rb");
        rapidjson::FileReadStream is(apiFile, buffer, sizeof(buffer));
        rapidjson::Document doc;
        doc.ParseStream(is);
        if (doc.HasParseError()) {
            std::cerr << "Parse Error!" << std::endl;
        }
        int maxNumberOfPages = doc["meta"]["last_page"].GetInt();
        fclose(apiFile);

        // Get json file with the random page
        apiFile = fopen(apiFilePath.c_str(), "w");
        srand(time(NULL));
        url += "&page=" + std::to_string(1 + rand() % maxNumberOfPages);  // make url with a random page
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());                       // in range [1, maxNumberOfPages]
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, apiFile);
        err = curl_easy_perform(curl);
        if (err != CURLE_OK) {
            std::cerr << "Curl_easy_setopt error! \n" << curl_easy_strerror(err) << std::endl;
        }
        curl_easy_reset(curl);
        fclose(apiFile);
    }
    curl_easy_cleanup(curl);
    getURL.close();
    return numberOfURLs; // number of URL's = number of JSON files
}

