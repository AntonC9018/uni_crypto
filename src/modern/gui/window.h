#pragma once
#include <gtkmm/box.h>
#include <gtkmm/notebook.h>
#include "trivium.h"
#include "salsa20.h"
#include "rijndael.h"

class ModernAlgorithmsPage : public Gtk::Box
{
public:
    ModernAlgorithmsPage();

private:
    Gtk::Notebook m_AlgoWindows;
    Gtk::Label m_MainMessage;
    Salsa20Box m_Salsa20Box;
    TriviumBox m_TriviumBox;
    RijndaelBox m_RijndaelBox;
};
