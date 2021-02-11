#pragma once
#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/separator.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/textview.h>
#include <strlib.h>
#include "../algos/vigenere.h"
#include "common.h"

class VigenereBox : public Gtk::Box
{
public:
    VigenereBox();

private:
    bool m_ignoreAnyInput = false;
    str_t m_keyword = {0};

    void changed_keyword();
    void changed_message_text();
    void changed_encrypted_text();

    bool validate();

    void do_encrypt();
    void do_decrypt();

    void recreate_encryption_grid();
    void recreate_key_grid();

    // Layout Stuff
    // Key
    Gtk::Grid m_KeyGrid;
    Gtk::Label m_KeyHeader;
    
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

    // Grids
    Gtk::Grid m_KeyTable_Grid;             // Displays Caesar permutations of the letters in the keyword.
    Gtk::Box  m_EncryptionTable_Box;
    Gtk::Grid m_EncryptionTableLabel_Grid;
    Gtk::Grid m_EncryptionTable_Grid;      // Displays the letter by letter process of encryption.
};