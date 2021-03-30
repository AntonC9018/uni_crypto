#pragma once
#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/separator.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/textview.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/radiobutton.h>
#include <glibmm.h>
#include <shared/logger.h>
#include "../algos/rijndael.h"


class RijndaelBox : public Gtk::Box
{
public:
    RijndaelBox();

private:

    // Algorithm Logic
    bool m_ignoreTextInput;

    // Layout Stuff
    // Key
    Gtk::Grid m_KeyGrid;
    Gtk::Label m_KeyHeader;
    
    Gtk::Label m_KeywordLabel;
    Gtk::Entry m_KeywordEntry;
    Glib::RefPtr<Gtk::EntryBuffer> m_refKeywordBuffer;

    Gtk::Grid m_ModeGrid;
    Gtk::Label m_ModeLabel;

    Gtk::Label m_128ModeLabel;
    Gtk::RadioButton m_128ModeRadioButton;

    Gtk::Label m_192ModeLabel;
    Gtk::RadioButton m_192ModeRadioButton;

    Gtk::Label m_256ModeLabel;
    Gtk::RadioButton m_256ModeRadioButton;

    // Usage
    Gtk::Grid m_UsageGrid;
    Gtk::Label m_UsageHeader;

    Gtk::TextView m_PlainTextView;
    Gtk::Label m_PlainTextLabel;
    Glib::RefPtr<Gtk::TextBuffer> m_refPlainTextBuffer;

    Gtk::TextView m_EncryptedTextView;
    Gtk::Label m_EncryptedTextLabel;
    Glib::RefPtr<Gtk::TextBuffer> m_refEncryptedTextBuffer;

    // Logic
    void changed_mode();
    void changed_message_text();
    void changed_encrypted_text();
    void changed_keyword_text();

    AES_CYPHER_T get_selected_mode();

    void validate(const Glib::ustring& buffer);

    void encrypt(const Glib::ustring& buffer);
    Glib::ustring crypt(const Glib::ustring& buffer);
    void decrypt(const Glib::ustring& buffer);

    Logger logger;
};