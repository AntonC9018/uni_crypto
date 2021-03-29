#pragma once
#include <strlib.h>
#include <shared/logger.h>

class RjindaelBox : public Gtk::Box
{
public:
    RjindaelBox();

private:

    // Algorithm Logic
    bool m_ignoreTextInput;
};