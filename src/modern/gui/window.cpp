#include "window.h"

ModernMainWindow::ModernMainWindow()
    : m_MainMessage("Select an algorithm from the pads above")
{
    add(m_AlgoWindows);
    m_AlgoWindows.append_page(m_MainMessage, "Main Page");
    m_AlgoWindows.append_page(m_Salsa20Box, "Salsa20");
    m_AlgoWindows.append_page(m_TriviumBox, "Trivium");
    auto page = m_AlgoWindows.append_page(m_RijndaelBox, "Rjindael");

    show_all_children();
    m_AlgoWindows.set_current_page(page);
}

ModernMainWindow::~ModernMainWindow() {}
