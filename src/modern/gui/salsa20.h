#pragma once
#include <strlib.h>
#include <shared/logger.h>

class Salsa20Box : public Gtk::Box
{
public:
    Salsa20Box();

private:

    // Algorithm Logic
    bool m_ignoreTextInput;
};