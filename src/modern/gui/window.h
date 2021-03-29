#pragma once
#include <gtkmm/window.h>
#include <gtkmm/notebook.h>
#include "trivium.h"
#include "salsa20.h"
#include "rjindael.h"

class ModernMainWindow : public Gtk::Window
{
public:
    ModernMainWindow();
    virtual ~ModernMainWindow();

private:
    Gtk::Notebook m_AlgoWindows;
    Gtk::Label m_MainMessage;
    Salsa20Box m_Salsa20Box;
    TriviumBox m_TriviumBox;
    RjindaelBox m_RjindaelBox;
};