#define CURL_STATICLIB
#include <vector>
#include <fstream>
#include <string>

#include "Gui.h"

#include "rapidjson/document.h"

int main(int argc, char** argv) {
    Gui win("AutoWallpapers");
    return Fl::run();
}