#pragma once
#include <gtkmm/box.h>
#include <gtkmm/grid.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/separator.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/textview.h>
#include <strlib.h>
#include "../algos/shift.h"
#include "../algos/common.h"
#include "common.h"
#include "../logger.h"

class ShiftBox : public Gtk::Box
{
public:
    ShiftBox();

private:

    // Algorithm Logic
    // std::vector<str_t> m_errors;
    Shift::Key m_key;
    bool m_ignoreTextInput;
    bool m_ignoreDimensionChange;

    void validate();
    void add_error();
    void do_crypto(Gtk::TextBuffer* buffer);
    void recreate_grids();

    // Handlers
    void changed_text(Gtk::TextBuffer* buffer);
    void changed_perm_entry(Gtk::Entry* entry, Gtk::Adjustment* adj);

    // Layout Stuff
    // Key
    Gtk::Grid m_KeyGrid;
    Gtk::Label m_KeyHeader;
    
    Gtk::Label m_WidthLabel;
    Glib::RefPtr<Gtk::Adjustment> m_WidthAdjustment;
    Gtk::SpinButton m_WidthButton;

    Gtk::Label m_HeightLabel;
    Glib::RefPtr<Gtk::Adjustment> m_HeightAdjustment;
    Gtk::SpinButton m_HeightButton;

    Gtk::Label m_RowPermLable;
    Gtk::Entry m_RowPermEntry;

    Gtk::Label m_ColPermLable;
    Gtk::Entry m_ColPermEntry;

    Gtk::Separator m_KeySeparator;

    // Usage
    Gtk::Grid m_UsageGrid;
    Gtk::Label m_UsageHeader;

    Gtk::Label m_CharsLeftLabel;

    Gtk::TextView m_PlainTextView;
    Gtk::Label m_PlainTextLabel;
    Glib::RefPtr<Gtk::TextBuffer> m_refPlainTextBuffer;

    Gtk::TextView m_EncryptedTextView;
    Gtk::Label m_EncryptedTextLabel;
    Glib::RefPtr<Gtk::TextBuffer> m_refEncryptedTextBuffer;

    // Grids
    Gtk::Box m_GridsBox;

    Gtk::Grid m_PlainGrid;
    Gtk::Grid m_EncryptedGrid;

    // Errors
    Logger logger;
};