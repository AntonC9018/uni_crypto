#include "window.h"

ClassicAlgorithmsPage::ClassicAlgorithmsPage()
    : Gtk::Box(Gtk::ORIENTATION_VERTICAL),
      m_MainMessage("Select an algorithm from the tabs above")
{
    pack_start(m_AlgoWindows);
    m_AlgoWindows.append_page(m_MainMessage, "Main Page");
    m_AlgoWindows.append_page(m_ShiftBox, "Shift");
    m_AlgoWindows.append_page(m_StraddlingBox, "Straddling");
    m_AlgoWindows.append_page(m_VigenereBox, "Vigenere");
    m_AlgoWindows.append_page(m_BazeriesBox, "Bazeries");
}
