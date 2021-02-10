#pragma once
#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/separator.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/textview.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/radiobutton.h>
#include <strlib.h>
#include "../algos/straddling.h"
#include "../algos/common.h"
#include "../shared.h"
#include "common.h"

class StraddlingBox : public Gtk::Box
{
public:
    StraddlingBox();

private:
    // Logic
    // First order
    str_t m_alphabet = {};
    str_t m_keyword = {};
    str_t m_scramble = {};
    std::vector<size_t> m_row_indices = {};
    // Second order (dependent)
    str_t m_char_set = {};
    std::vector<size_t> m_order = {};
    // Whether can generate
    bool m_valid = false;
    bool m_ignoreAnyInput = false;
    // Display setting: group size
    bool m_show_in_groups = true;
    size_t m_group_size = 5;

    Straddling::Key m_key = {};

    bool validate();    

    void changed_keyword();     // -> keyword, charset
    void changed_scramble();    // -> order
    void changed_row_indices(); // -> row_indices
    // Note that the influence -> charset is satisfied transitively through keyword
    void changed_alphabet();    // :: keyword, scramble -> order; 
    void changed_text(Gtk::TextBuffer* text_buffer);    // :: keyword, scramble -> order; 
    void changed_show();
    void changed_group_size();

    void refresh_alphabet();
    void refresh_keyword();     // alphabet -> keyword
    void refresh_scramble();    // alphabet -> scramble
    void refresh_charset();     // keyword -> charset
    void refresh_order();       // accounts for scramble -> order dependency
    void refresh_row_indices();

    void make_key();
    void do_crypto(Gtk::TextBuffer* text_buffer);
    void recreate_grid();       // influenced by key

    void leave_unique_in_alphabet(Gtk::Entry& entry, str_t& current);
    
    // Key
    Gtk::Grid m_KeyGrid;
    Gtk::Label m_KeyHeader;

    Gtk::Label        m_AlphabetLabel;
    Gtk::ComboBoxText m_AlphabetCombo;

    Gtk::Label m_RowIndicesLabel;
    Gtk::Entry m_RowIndicesEntry;

    Gtk::Label m_ScrambleLabel;
    Gtk::Entry m_ScrambleEntry;

    Gtk::Label m_KeywordLabel;
    Gtk::Entry m_KeywordEntry;

    // Usage
    Gtk::Grid m_UsageGrid;
    Gtk::Label m_UsageHeader;

    Gtk::TextView m_PlainTextView;
    Gtk::Label m_PlainTextLabel;
    Glib::RefPtr<Gtk::TextBuffer> m_refPlainTextBuffer;

    Gtk::TextView m_EncryptedTextView;
    Gtk::Label m_EncryptedTextLabel;
    Glib::RefPtr<Gtk::TextBuffer> m_refEncryptedTextBuffer;

    Gtk::RadioButton               m_EncryptedDisplay_GroupsButton;
    Gtk::SpinButton                m_EncryptedDisplay_GroupLengthButton;
    Glib::RefPtr<Gtk::Adjustment>  m_refGroupLengthAdjustment;
    Gtk::RadioButton               m_EncryptedDisplay_ConcatenateButton;

    // Table
    Gtk::Grid m_TableGrid;
};