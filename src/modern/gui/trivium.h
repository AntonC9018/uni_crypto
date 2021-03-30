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


class TriviumBox : public Gtk::Box
{
public:
    TriviumBox();

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

    Gtk::Label m_InitializationVectorLabel;
    Gtk::Entry m_InitializationVectorEntry;
    Glib::RefPtr<Gtk::EntryBuffer> m_refInitializationVectorBuffer;

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
    void changed_message_text();
    void changed_ivector_text();
    void changed_encrypted_text();
    void changed_keyword_text();

    void validate();

    void encrypt();
    Glib::ustring crypt(const Glib::ustring& buffer);
    void decrypt();

    Logger logger;
};