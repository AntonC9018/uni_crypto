#pragma once
#include <gtkmm/window.h>
#include <gtkmm/notebook.h>
#include "shift.h"
#include "straddling.h"

class MainWindow : public Gtk::Window
{
public:
    MainWindow();
    virtual ~MainWindow();

private:
    Gtk::Notebook m_AlgoWindows;
    Gtk::Label m_MainMessage;
    ShiftBox m_ShiftBox;
    StraddlingBox m_StraddlingBox;
};