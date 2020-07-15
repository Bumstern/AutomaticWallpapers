#define CURL_STATICLIB
#include "Gui.h"
#include <direct.h>

int main(int argc, char** argv) {
    _mkdir("tempFiles");
    Gui win("AutoWallpapers");
    return Fl::run();
}