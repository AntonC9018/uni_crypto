#pragma once
#include <gtkmm/widget.h>
#include <gtkmm/grid.h>
#include <gtkmm/label.h>

void remove_widget(Gtk::Widget& widget);
void grid_remove_children(Gtk::Grid& grid);
void attach_label(char text, int col, int row, Gtk::Grid& parent);