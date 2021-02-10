#include "common.h"
#include <cairomm/fontoptions.h>
void remove_widget(Gtk::Widget& widget) 
{ 
    widget.get_parent()->remove(widget); 
}

void grid_remove_children(Gtk::Grid& grid)
{
    grid.forall(sigc::ptr_fun(remove_widget));
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
    char b[2] = { text, 0 };
    t->set_text(b);
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