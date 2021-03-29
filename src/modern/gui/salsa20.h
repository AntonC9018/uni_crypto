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


class Salsa20Box : public Gtk::Box
{
public:
    Salsa20Box();

private:

    // Algorithm Logic
    bool m_ignoreTextInput;

    enum Mode { _16_BYTE, _32_BYTE };

    // Layout Stuff
    // Key
    Gtk::Grid m_KeyGrid;
    Gtk::Label m_KeyHeader;
    
    Gtk::Label m_KeywordLabel;
    Gtk::Entry m_KeywordEntry;
    Glib::RefPtr<Gtk::EntryBuffer> m_refKeywordBuffer;

    Gtk::Label m_NonceLabel;
    Gtk::Entry m_NonceEntry;
    Glib::RefPtr<Gtk::EntryBuffer> m_refNonceBuffer;

    Gtk::Label m_ModeLabel;

    Gtk::Label m_16ByteLabel;
    Gtk::RadioButton m_16ByteRadioButton;

    Gtk::Label m_32ByteLabel;
    Gtk::RadioButton m_32ByteRadioButton;

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
    void changed_nonce_text();
    void changed_encrypted_text();
    void changed_keyword_text();

    void validate();

    void encrypt();
    Glib::ustring crypt(const Glib::ustring& buffer);
    void decrypt();

    Logger logger;
};