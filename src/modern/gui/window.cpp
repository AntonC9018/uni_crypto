#include "window.h"

ModernAlgorithmsPage::ModernAlgorithmsPage()
    : Gtk::Box(Gtk::ORIENTATION_VERTICAL),
      m_MainMessage("Select an algorithm from the tabs above")
{
    pack_start(m_AlgoWindows);
    m_AlgoWindows.append_page(m_MainMessage, "Main Page");
    m_AlgoWindows.append_page(m_Salsa20Box, "Salsa20");
    m_AlgoWindows.append_page(m_TriviumBox, "Trivium");
    m_AlgoWindows.append_page(m_RijndaelBox, "Rijndael");
}
