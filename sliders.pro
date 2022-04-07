TEMPLATE = app
QT += widgets

extralib.target = extra
extralib.commands = echo "Precompiling libpd and portaudio.."; \
                            $$PWD/makeportaudioandlibpd.sh
extralib.depends =

QMAKE_EXTRA_TARGETS += extralib
PRE_TARGETDEPS = extra

QMAKE_CC=clang
QMAKE_CXX=clang++
CONFIG += static
CONFIG += c++11

INCLUDEPATH += $$PWD/libpd/libpd_wrapper
INCLUDEPATH += $$PWD/libpd/pure-data/src
INCLUDEPATH += portaudio/include

macx
{
    LIBS += -framework AudioToolbox \
            -framework AudioUnit \
            -framework CoreAudio \
            -framework CoreServices \
            -framework Accelerate \
            -framework Carbon \
}

HEADERS     = \
              window.h \
    pawrapper.h \
    audiooutput.h \

SOURCES     = main.cpp \
              window.cpp \
    pawrapper.cpp \
    audiooutput.cpp \


installs.files += $$PWD/libpd/libs/libpd.dylib
installs.files += $$PWD/portaudio/lib/.libs/libportaudio.a
installs.path = $$OUT_PWD/sliders.app/Contents/MacOS
INSTALLS += installs

copydata.commands = $(COPY_FILE) $$PWD/portaudio/lib/.libs/libportaudio.2.dylib /usr/local/lib/ \
&& $(COPY_FILE) $$PWD/libpd/libs/libpd.dylib /Users/harveykeitel/libs \

first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)

QMAKE_EXTRA_TARGETS += first copydata

target.path = $$PWD/build
INSTALLS += target

macx: LIBS += -L$$PWD/portaudio/lib/.libs/ -lportaudio
macx: LIBS += -L$$PWD/libpd/libs/ -lpd

