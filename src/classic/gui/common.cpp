#include "common.h"

void grid_remove_children(Gtk::Grid& grid)
{
    auto childList = grid.get_children();
    auto it = childList.begin();

    while (it != childList.end()) 
    {
        grid.remove(**it);
        delete *it;
        it++;
    }
}

static Pango::FontDescription consolas_font("Consolas normal 12");

Gtk::Label* attach_label(str_view_t text, int col, int row, Gtk::Grid& parent)
{
    auto t = Gtk::make_managed<Gtk::Label>();
    parent.attach(*t, col + 1, row + 1, 1, 1);
    t->set_text({ text.chars, text.length });
    t->override_font(consolas_font);
    t->set_width_chars(2);
    t->show();
    return t;
}

Gtk::Label* attach_label(char text, int col, int row, Gtk::Grid& parent)
{
    auto t = Gtk::make_managed<Gtk::Label>();
    parent.attach(*t, col + 1, row + 1, 1, 1);
    t->set_text({ &text, 1 });
    t->override_font(consolas_font);
    t->set_width_chars(2);
    t->show();
    return t;
}

Gtk::Label* attach_bold_label(char text, int col, int row, Gtk::Grid& parent)
{
    auto t = Gtk::make_managed<Gtk::Label>();
    parent.attach(*t, col + 1, row + 1, 1, 1);
    char b[] = { '<', 'b', '>', text, '<', '/', 'b', '>' };
    t->set_markup({ b, sizeof(b) });
    t->override_font(consolas_font);
    t->set_width_chars(2);
    t->show();
    return t;
}

str_t keep_latin_to_upper(const Glib::ustring& keyword_text)
{
    auto sb = strb_make(keyword_text.size());
    for (size_t i = 0; i < keyword_text.size(); i++)
    {
        if (keyword_text.at(i) >= 'a' && keyword_text.at(i) <= 'z')
        {
            // Lower case all letters
            strb_chr(sb, keyword_text.at(i) - 'a' + 'A');
        }
        else if (keyword_text.at(i) >= 'A' && keyword_text.at(i) <= 'Z')
        {
            strb_chr(sb, keyword_text.at(i));
        }
    }
    return strb_build(sb);
}

void reset_keyword_from_entry_latin_upper(Gtk::Entry* entry, str_t* keyword)
{
    auto gtk_message = entry->get_text();
    str_t new_keyword = keep_latin_to_upper(gtk_message);
    entry->set_text({ new_keyword.chars, new_keyword.length });
    if (keyword->chars) { str_free(*keyword); }
    *keyword = new_keyword;
}

void reset_textbuffer_latin_upper(Gtk::TextBuffer* textbuffer)
{
    // This does at least two copies of the source string
    auto gtk_message = textbuffer->get_text();
    str_t new_text = keep_latin_to_upper(gtk_message);
    // This copies it at least 2 more times
    textbuffer->set_text({ new_text.chars, new_text.length });
    str_free(new_text);
}

// void reset_textbuffer_latin_upper_letter_map(Gtk::TextBuffer* textbuffer, LetterMap mapf)
// {
    
// }