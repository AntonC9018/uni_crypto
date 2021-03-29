#pragma once
#include <strlib.h>
#include <shared/logger.h>

class TriviumBox : public Gtk::Box
{
public:
    TriviumBox();

private:

    // Algorithm Logic
    bool m_ignoreTextInput;
};