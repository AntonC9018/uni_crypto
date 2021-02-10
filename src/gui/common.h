#pragma once
#include <gtkmm/widget.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>
#include <strlib.h>

void remove_widget(Gtk::Widget& widget);
void grid_remove_children(Gtk::Grid& grid);
Gtk::Label* attach_label(char text, int col, int row, Gtk::Grid& parent);
Gtk::Label* attach_label(str_view_t text, int col, int row, Gtk::Grid& parent);
Gtk::Label* attach_bold_label(char text, int col, int row, Gtk::Grid& parent);
