#pragma once
#include <strlib.h>
#include <gtkmm/textbuffer.h>
#include <gtkmm/textview.h>
#include <gtkmm/box.h>

struct Logger
{
    bool has_errors;
    Gtk::TextView textview;
    Glib::RefPtr<Gtk::TextBuffer> textbuffer;

    Logger()
    {
        has_errors = false;
        textbuffer = Gtk::TextBuffer::create();
        textview.set_buffer(textbuffer);
    }
};

inline void logger_attach(Logger* logger, Gtk::Box* parent)
{
    parent->pack_start(logger->textview);
}

inline void logger_clear(Logger* logger)
{
    logger->textview.hide();
    logger->textbuffer->set_text("");
    logger->has_errors = false;
}

inline void logger_add_error(Logger* logger, str_view_t str)
{
    logger->textview.show();
    logger->textbuffer->insert_interactive_at_cursor(str.chars, str.chars + str.length);
    logger->has_errors = true;
}

#define logger_format_error(logger, string, ...) \
{ \
    char _buff[256]; \
    size_t _num_chars = sprintf(_buff, (string), ## __VA_ARGS__); \
    logger_add_error((logger), { _buff, _num_chars }); \
}
