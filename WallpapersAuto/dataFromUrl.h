#pragma once
#define CURL_STATICLIB
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>

#include "curl/curl.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

void urlGen(); // URL generation

int getJson(); // Get Json files from URL's