#include "common.h"

void remove_widget(Gtk::Widget& widget) 
{ 
    widget.get_parent()->remove(widget); 
}

void grid_remove_children(Gtk::Grid& grid)
{
    grid.forall(sigc::ptr_fun(remove_widget));
}

void attach_label(char text, int col, int row, Gtk::Grid& parent)
{
    auto t = Gtk::make_managed<Gtk::Label>();
    parent.attach(*t, col + 1, row + 1, 1, 1);
    char b[2] = { text, 0 };
    t->set_text(b);
    t->show();
}