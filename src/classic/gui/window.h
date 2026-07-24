#pragma once
#include <gtkmm/box.h>
#include <gtkmm/notebook.h>
#include "shift.h"
#include "straddling.h"
#include "vigenere.h"
#include "bazeries.h"

class ClassicAlgorithmsPage : public Gtk::Box
{
public:
    ClassicAlgorithmsPage();

private:
    Gtk::Notebook m_AlgoWindows;
    Gtk::Label m_MainMessage;
    ShiftBox m_ShiftBox;
    StraddlingBox m_StraddlingBox;
    VigenereBox m_VigenereBox;
    BazeriesBox m_BazeriesBox;
};
