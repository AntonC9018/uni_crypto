#include "window.h"

MainWindow::MainWindow()
{
    add(m_AlgoWindows);
    m_AlgoWindows.append_page(m_MainMessage, "ALGORITHMS");
    m_AlgoWindows.append_page(m_ShiftBox, "Shift");
    
    show_all_children();
}

MainWindow::~MainWindow() {}
