#include "window.h"

MainWindow::MainWindow()
{
    set_title("Cryptography");
    set_default_size(900, 700);

    add(m_AlgorithmPages);
    m_AlgorithmPages.append_page(m_ClassicAlgorithms, "Classic Algorithms");
    m_AlgorithmPages.append_page(m_ModernAlgorithms, "Modern Algorithms");

    show_all_children();
}
