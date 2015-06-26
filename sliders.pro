QT += widgets

LIBS += $$_PRO_FILE_PWD_/libpd/libs/libpd-osx.a
LIBS += $$_PRO_FILE_PWD_/portaudio/lib/.libs/libportaudio.a
INCLUDEPATH += $$_PRO_FILE_PWD_/libpd/libpd_wrapper
INCLUDEPATH += $$_PRO_FILE_PWD_/libpd/pure-data/src
INCLUDEPATH += $$_PRO_FILE_PWD_/portaudio/include
INCLUDEPATH += .

macx
{
    QMAKE_MAC_SDK = macosx10.9
    LIBS += -framework AudioToolbox \
            -framework AudioUnit \
            -framework CoreAudio \
            -framework CoreServices \
            -framework Carbon
}

HEADERS     = \
              window.h \
    pawrapper.h \
    audiooutput.h \

SOURCES     = main.cpp \
              window.cpp \
    pawrapper.cpp \
    audiooutput.cpp \

QMAKE_MAC_SDK = macosx10.9

# install
target.path = $$_PRO_FILE_PWD_/build
INSTALLS += target

copydata.commands = $(COPY_DIR) $$PWD/puredata $$OUT_PWD/sliders.app/Contents/MacOS
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

