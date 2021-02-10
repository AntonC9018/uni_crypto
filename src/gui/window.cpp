#include "window.h"

MainWindow::MainWindow()
{
    add(m_AlgoWindows);
    m_AlgoWindows.append_page(m_MainMessage, "Main Page");
    m_AlgoWindows.append_page(m_ShiftBox, "Shift");
    m_AlgoWindows.append_page(m_StraddlingBox, "Straddling");
    auto page = m_AlgoWindows.append_page(m_VigenereBox, "Vigenere");

    show_all_children();
    m_AlgoWindows.set_current_page(page);
}

MainWindow::~MainWindow() {}
