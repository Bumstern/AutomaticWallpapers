#pragma once
#define CURL_STATICLIB
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <stdio.h>
#include <ctime>

#include"curl/curl.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

bool fileExists(char* fname);

void getDataFromJson(int number);