#pragma once

#include <gtkmm/notebook.h>
#include <gtkmm/window.h>

#include "classic/gui/window.h"
#include "modern/gui/window.h"

class MainWindow : public Gtk::Window
{
public:
    MainWindow();

private:
    Gtk::Notebook m_AlgorithmPages;
    ClassicAlgorithmsPage m_ClassicAlgorithms;
    ModernAlgorithmsPage m_ModernAlgorithms;
};
