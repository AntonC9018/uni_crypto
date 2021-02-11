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
#include "../algos/bazeries.h"
#include "../algos/common.h"
#include "../shared.h"
#include "common.h"
#include "../logger.h"

struct Message
{
    str_t plain;
    str_t delimited;
};

class BazeriesBox : public Gtk::Box
{
public:
    BazeriesBox();

private:
    // Logic
    // First order
    str_t             m_keyword = {0};
    std::vector<char> m_numeric_keyword;
    Bazeries::Key m_key;
    bool m_ignoreAnyInput = true;

    Message m_plain_message = {0};
    Message m_encrypted_message = {0};

    void changed_keyword();
    void changed_numeric_keyword();
    void changed_message_text();
    void changed_encrypted_text();

    void refresh_keyword();
    void refresh_numeric_keyword();
    void clear_message(Message message);
    void remake_key_and_encrypt();
    void reread_plain_and_encrypt();
    Message refresh_text(Gtk::TextBuffer* textbuffer);

    void recreate_grid();

    void validate();
    void make_key();
    void do_encrypt();
    void do_decrypt();
    
    // Key
    Gtk::Grid m_KeyGrid;
    Gtk::Label m_KeyHeader;

    Gtk::Label m_KeywordLabel;
    Gtk::Entry m_KeywordEntry;

    Gtk::Label m_NumericKeywordLabel;
    Gtk::Entry m_NumericKeywordEntry;

    // Usage
    Gtk::Grid m_UsageGrid;
    Gtk::Label m_UsageHeader;

    Gtk::TextView m_PlainTextView;
    Gtk::Label m_PlainTextLabel;
    Glib::RefPtr<Gtk::TextBuffer> m_refPlainTextBuffer;

    Gtk::TextView m_EncryptedTextView;
    Gtk::Label m_EncryptedTextLabel;
    Glib::RefPtr<Gtk::TextBuffer> m_refEncryptedTextBuffer;

    // Table
    Gtk::Box  m_TableBox;
    Gtk::Grid m_TransposedAlphabet_Grid;
    Gtk::Grid m_PolybiosTable_Grid;

    // Errors
    Logger logger;
};