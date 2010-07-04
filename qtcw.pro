#-------------------------------------------------
#
# Project created by QtCreator 2010-07-01T21:21:28
#
#-------------------------------------------------

QT       += core gui multimedia

TARGET = qtcw
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
        Morse.cpp \
    Generator.cpp \
    MorseStat.cpp

HEADERS  += MainWindow.h \
    Morse.h \
    morse_code.h \
    Generator.h \
    MorseStat.h

FORMS    += MainWindow.ui

CONFIG += mobility
MOBILITY = 

symbian {
    TARGET.UID3 = 0xe0df144b
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}
