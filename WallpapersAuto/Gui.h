#pragma once
#include <iostream>
#include <fstream>
#include <string> 
#include "resource.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Native_File_Chooser.H>

#include "backend.h"

// Counts amounts of lines 
int countLines(std::string s);

// Check path to file for correctness
bool checkFilePath(const char* fname);

class Gui : public Fl_Window  {
private:
    Fl_Menu_Bar* menuBar;
    Fl_Multiline_Input* tagInput;
    Fl_Check_Button* sfmButton;
    Fl_Check_Button* sketchyButton;
    Fl_Check_Button* nsfwButton;
    Fl_Input_Choice* ratioInput;
    Fl_Value_Input* amountInput;
    Fl_Round_Button* randomButton;
    Fl_Button* resultButton;

    static void apiChangeCallback(Fl_Widget* w, void*);     // change API
    static void pathChooseCallback(Fl_Widget* w, void*);    // choose path to save-catalog 
    static void instructionCallback(Fl_Widget* w, void*);   // pop-up window how to use 
    static void aboutCallback(Fl_Widget* w, void*);         // pop-up window about author
    static void tagCallback(Fl_Widget* w, void* data);      // tags input
    static void ratioCheckCallback(Fl_Widget* w);           // check ratio button input
    static void amountCheckCallback(Fl_Widget* w);          // check amount button input
    static void resultCallback(Fl_Widget* w, void* data);   // get data from all widgets using result button

public:
    Gui(const char* name = 0);
};

