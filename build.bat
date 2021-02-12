@echo off

"D:/msys64/mingw64/bin/g++.exe" -Wall -std=c++17 -pthread^
 ./src/*.cpp ./src/algos/*.cpp ./third_party/strlib/strlib.cpp ^
 -Ithird_party -Ithird_party/mydefines -Ithird_party/strlib -I../includes ^
 -ID:/msys64/mingw64/include/gtkmm-3.0 ^
 -ID:/msys64/mingw64/lib/gtkmm-3.0/include ^
 -ID:/msys64/mingw64/include/atkmm-1.6 ^
 -ID:/msys64/mingw64/include/atk-1.0 ^
 -ID:/msys64/mingw64/include/glib-2.0 ^
 -ID:/msys64/mingw64/lib/glib-2.0/include ^
 -ID:/msys64/mingw64/include ^
 -ID:/msys64/mingw64/include/glibmm-2.4 ^
 -ID:/msys64/mingw64/lib/glibmm-2.4/include ^
 -ID:/msys64/mingw64/include/sigc++-2.0 ^
 -ID:/msys64/mingw64/lib/sigc++-2.0/include ^
 -ID:/msys64/mingw64/include/giomm-2.4 ^
 -ID:/msys64/mingw64/lib/giomm-2.4/include -mms-bitfields ^
 -ID:/msys64/mingw64/include/pangomm-1.4 ^
 -ID:/msys64/mingw64/lib/pangomm-1.4/include ^
 -ID:/msys64/mingw64/include/cairomm-1.0 ^
 -ID:/msys64/mingw64/lib/cairomm-1.0/include ^
 -ID:/msys64/mingw64/include/cairo ^
 -ID:/msys64/mingw64/include/lzo ^
 -ID:/msys64/mingw64/include/libpng16 ^
 -ID:/msys64/mingw64/include/freetype2 ^
 -ID:/msys64/mingw64/include/harfbuzz ^
 -ID:/msys64/mingw64/include/pixman-1 ^
 -ID:/msys64/mingw64/include/pango-1.0 -mms-bitfields ^
 -ID:/msys64/mingw64/include/fribidi -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields ^
 -ID:/msys64/mingw64/include/gtk-3.0 -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields ^
 -ID:/msys64/mingw64/include/gdk-pixbuf-2.0 -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields ^ -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields ^
 -ID:/msys64/mingw64/include/gdkmm-3.0 ^
 -ID:/msys64/mingw64/lib/gdkmm-3.0/include -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -mms-bitfields -pthread -mms-bitfields -LD:/msys64/mingw64/bin/../lib -lgtkmm-3.0 -latkmm-1.6 -lgdkmm-3.0 -lgiomm-2.4 -lpangomm-1.4 -lglibmm-2.4 -lgtk-3 -lgdk-3 -lz -lgdi32 -limm32 -lshell32 -lole32 -Wl,-luuid -lwinmm -ldwmapi -lsetupapi -lcfgmgr32 -lpangowin32-1.0 -lpangocairo-1.0 -lpango-1.0 -lharfbuzz -latk-1.0 -lcairo-gobject -lgio-2.0 -lcairomm-1.0 -lcairo -lsigc-2.0 -lgdk_pixbuf-2.0 -lgobject-2.0 -lglib-2.0 -lintl