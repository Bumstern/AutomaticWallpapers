#include "Gui.h"

// Check Ethernet connection
bool checkEthernetConnection() {
    CURL* curl;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.ru/");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    if (curl_easy_perform(curl) == CURLE_OK) {
        curl_easy_cleanup(curl);
        return true;
    }
    else {
        curl_easy_cleanup(curl);
        return false;
    }
}

// Counts amounts of lines 
int countLines(std::string s) {
    int res = 1;
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '\n') {
            ++res;
        }
    }
    return res;
}

// Check path to file for correctness
bool checkFilePath(const char* fname) {
    int i = 0;
    while (fname[i] != '\0') { 
        if ((fname[i] > 'à' && fname[i] < 'ÿ') || // Check for russian characters
            (fname[i] > 'À' && fname[i] < 'ß')) {
            return false;
        }
        ++i;
    }
    return true;
}

// Change API
void Gui::apiChangeCallback(Fl_Widget* w, void*) {
    std::ifstream in("tempFiles/api.txt");
    char api[100] = "Enter_Api";
    if (in.is_open() && in.peek() != std::ifstream::traits_type::eof()) {
        in >> api;
    }
    in.close();

    const char* input = fl_input("Enter API:", api);

    if (input != NULL) {
        std::ofstream out("tempFiles/api.txt");
        out << input;
        out.close();
    }
}

// Choose path to save-catalog 
void Gui::pathChooseCallback(Fl_Widget* w, void*) {
    Fl_Native_File_Chooser path;
    path.title("Choose path");
    path.type(Fl_Native_File_Chooser::BROWSE_DIRECTORY);

    std::ifstream in("tempFiles/path.txt");
    std::string dir;
    in >> dir;
    in.close();

    std::ofstream out("tempFiles/path.txt");
    switch (path.show()) {
    case -1:
        out << "NULL";
        break;
    case 1:
        out << dir;
        break;
    default:
        if (path.filename()) {
            if (checkFilePath(path.filename())) 
                out << path.filename();
            else {
                fl_alert("Please enter the pathname using only english-characters!");
                out << "NULL";
            }
        }
        else 
            out << "NULL";
        break;
    }
    out.close();
}

// pop-up window how to use
void Gui::instructionCallback(Fl_Widget* w, void*) {
    const char* text = 
        " 1) Choose pathname where you want to save your wallpapers.\n"
        " 2) Enter your \"API Key\" from site: https://wallhaven.cc/settings/account \n"
        "     in section \"Change API\".\n"
        " 3) Choose your purity, ratio and amount.\n"
        " 4) Enter each tag on a new line. (maximum 5 tags)\n"
        "     Or just choose \"random\" if you dont care about tags.\n";

    Fl_Double_Window* textWin = new Fl_Double_Window(500, 150, "How to use");
    Fl_Text_Display* textDisp = new Fl_Text_Display(0, 0, textWin->w(), textWin->h());
    Fl_Text_Buffer* textBuff = new Fl_Text_Buffer();    
    textDisp->buffer(textBuff);
    textDisp->textfont(FL_TIMES);
    textDisp->textsize(16);
    textBuff->text(text);
    textWin->show();
}

// pop-up window about author
void Gui::aboutCallback(Fl_Widget* w, void*) {
    fl_message("Automatic wallpapers\n%s\n%s", 
               "By Rusik",
               "15.07.2020");
}

// Tags input
void Gui::tagCallback(Fl_Widget* w, void* data) {
    Gui* gui = reinterpret_cast<Gui*>(data);
    std::string temp = gui->tagInput->value();

    if (countLines(temp) <= 5) { // max 5 tags
        std::ofstream out("tempFiles/tag.txt");
        out << gui->tagInput->value();
        out.close();
    }
}

// Check ratio button input
void Gui::ratioCheckCallback(Fl_Widget* w) {
    if (!((Fl_Input_Choice*)w)->menubutton()->find_item(((Fl_Input_Choice*)w)->value())) {
        ((Fl_Input_Choice*)w)->value("16x9");
    }
}

// Check amount button input
void Gui::amountCheckCallback(Fl_Widget* w) {
    if (((Fl_Value_Input*)w)->value() < 1) { // minimum bound 
        ((Fl_Value_Input*)w)->value(1);
    }
    else if (((Fl_Value_Input*)w)->value() > 10) { // maximum bound
        ((Fl_Value_Input*)w)->value(10);
    }
}

// Get data from all widgets using result button
void Gui::resultCallback(Fl_Widget* w, void* data) {
    Gui* gui = reinterpret_cast<Gui*>(data);

    // Check Ethernet connection
    if (checkEthernetConnection() == false) {
        fl_alert("Check your Ethernet connection and try again");
        return;
    }

    // Check for pathname input
    std::ifstream inPath("tempFiles/path.txt");
    std::string path;
    inPath >> path;
    if (path == "NULL") {
        fl_alert("Please enter the path.");
        return;
    }
    inPath.close();

    // Check for API input
    std::ifstream inAPI("tempFiles/api.txt");
    std::string api;
    inAPI >> api;
    if (api == "Enter_Api" || api == "") {
        fl_alert("Please enter the API.");
        return;
    }
    inAPI.close();

    // Check tags input 
    std::ifstream inTag("tempFiles/tag.txt");
    if (inTag.peek() == EOF && (int)gui->randomButton->value() == 0) {
        fl_alert("Please enter some tags or choose \"Random\"");
        return;
    }
    inTag.close();

    // Check purity input
    if ((int)gui->sfmButton->value() + (int)gui->sketchyButton->value() + (int)gui->nsfwButton->value() == 0) {
        fl_alert("Please choose one purity at least");
        return;
    }

    // Summarizes all data from widgets    
    std::ofstream out("tempFiles/result.txt");
    out << (int)gui->randomButton->value() << " ";
    out << (int)gui->sfmButton->value() << (int)gui->sketchyButton->value() << (int)gui->nsfwButton->value() << " ";
    out << gui->ratioInput->value() << " ";
    out << gui->amountInput->value() << " ";
    out.close();

    // Give control to function where take place main calculations 
    backend();
}

Gui::Gui(const char* name) : Fl_Window(497, 190, name) {
    //// Set positions of widgets ////
    begin();
    int x = 290, y = 0, h = 10;
    menuBar = new Fl_Menu_Bar(0, 0, 497, 25);

    tagInput = new Fl_Multiline_Input(50, 25 + h, 170, 100, "Tags:");
    y += 25 + h + 5;

    sfmButton = new Fl_Check_Button(x, y, 50, 15, "SFM");
    sketchyButton = new Fl_Check_Button(x + 60, y, 70, 15, "Sketchy");
    nsfwButton = new Fl_Check_Button(x + 140, y, 58, 15, "NSFM");
    y += 15 + h;

    ratioInput = new Fl_Input_Choice(x, y, 65, 25, "Ratio:");
    y += 25 + h;

    amountInput = new Fl_Value_Input(x, y, 25, 25, "Amount:");

    randomButton = new Fl_Round_Button(x + 60, y + 5, 75, 15, "Random");
    y += 15 + 2 * h + 5;

    resultButton = new Fl_Button(255 - 60 / 2, y, 60, 35, "GO!");
    end();

    //// Widgets initialization ////
    menuBar->add("Change API", FL_F + 1, apiChangeCallback);
    menuBar->add("Choose path", FL_F + 2, pathChooseCallback);
    menuBar->add("How to use", FL_F + 3, instructionCallback);
    menuBar->add("About", FL_F + 4, aboutCallback);
    
    std::ifstream inTag("tempFiles/tag.txt");
    std::string lastTags = "", temp;
    while (getline(inTag, temp)) { // input last tags
        lastTags += temp + "\n";
    }
    tagInput->value(lastTags.c_str());
    tagInput->when(FL_WHEN_RELEASE);
    tagInput->callback(tagCallback, this);

    sfmButton->value(1);
    sfmButton->tooltip("Safe For Work (0+)");
    sketchyButton->tooltip("Not for kids (16+)");
    nsfwButton->tooltip("Not Safe For Work (18+)");

    ratioInput->value("16x9");
    ratioInput->add("16x9");
    ratioInput->add("16x10");
    ratioInput->add("21x9");
    ratioInput->add("32x9");
    ratioInput->add("48x9");
    ratioInput->add("9x16");
    ratioInput->add("10x16");
    ratioInput->add("9x18");
    ratioInput->add("1x1");
    ratioInput->add("3x2");
    ratioInput->add("4x3");
    ratioInput->add("5x4");
    ratioInput->when(FL_WHEN_RELEASE);
    ratioInput->callback(ratioCheckCallback);

    amountInput->value(5);
    amountInput->when(FL_WHEN_RELEASE);
    amountInput->callback(amountCheckCallback);

    resultButton->callback(resultCallback, this);

    this->icon((const void*)LoadIcon(fl_display, MAKEINTRESOURCE(IDI_ICON1))); // set icon
    show();   
}
