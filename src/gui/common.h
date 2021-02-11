#pragma once
#include <gtkmm/widget.h>
#include <gtkmm/grid.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <cairomm/fontoptions.h>
#include <strlib.h>

void remove_widget(Gtk::Widget& widget);
void grid_remove_children(Gtk::Grid& grid);
Gtk::Label* attach_label(char text, int col, int row, Gtk::Grid& parent);
Gtk::Label* attach_label(str_view_t text, int col, int row, Gtk::Grid& parent);
Gtk::Label* attach_bold_label(char text, int col, int row, Gtk::Grid& parent);
str_t keep_latin_to_upper(const Glib::ustring& str);
void reset_keyword_from_entry_latin_upper(Gtk::Entry* entry, str_t* keyword);
void reset_textbuffer_latin_upper(Gtk::TextBuffer* textbuffer);