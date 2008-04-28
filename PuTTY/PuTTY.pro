TEMPLATE         = lib
CONFIG          += plugin release
UI_DIR           = .
FORMS            = putty_options.ui
HEADERS          = plugin_interface.h PuTTY.h OptionsDlg.h Options.h PuttySessions.h
SOURCES          = plugin_interface.cpp PuTTY.cpp OptionsDlg.cpp Options.cpp PuttySessions.cpp
TARGET           = putty

win32 {
        CONFIG -= embed_manifest_dll
        LIBS += shell32.lib
		LIBS += advapi32.lib
        LIBS += user32.lib
%       LIBS += Gdi32.lib
%       LIBS += comctl32.lib
}
 
*:debug {
        DESTDIR = ../debug/
}
*:release {
        DESTDIR = ../release/
        %QMAKE_CXXFLAGS += /Ox /Ob2 /Oi /Oy /GT /GA  /WX
}

