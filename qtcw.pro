#-------------------------------------------------
#
# Project created by QtCreator 2010-07-01T21:21:28
#
#-------------------------------------------------

QT       += core gui multimedia

TARGET = qtcw
TEMPLATE = app

RESOURCES = qtcw.qrc

SOURCES += main.cpp\
        MainWindow.cpp \
        Morse.cpp \
    Generator.cpp \
    MorseStat.cpp \
    Prefs.cpp \
    modes/SpeedTrainingMode.cpp \
    modes/TrainingMode.cpp \
    modes/MorseMode.cpp \
    modes/PlayMode.cpp \
    modes/LetterTrainingMode.cpp \
    modes/WordTrainingMode.cpp \
    modes/ReadMode.cpp \
    modes/ChooseSequenceDialog.cpp \
    modes/GroupingMode.cpp \
    modes/games/MGameMode.cpp \
    modes/games/MCountGameMode.cpp

HEADERS  += MainWindow.h \
    Morse.h \
    morse_code.h \
    Generator.h \
    MorseStat.h \
    Prefs.h \
    words/100words.h \
    words/500words.h \
    words/400words.h \
    words/300words.h \
    words/200words.h \
    modes/SpeedTrainingMode.h \
    modes/TrainingMode.h \
    modes/MorseMode.h \
    modes/PlayMode.h \
    modes/LetterTrainingMode.h \
    modes/WordTrainingMode.h \
    modes/ReadMode.h \
    modes/ChooseSequenceDialog.h \
    modes/GroupingMode.h \
    modes/games/MGameMode.cpp \
    modes/games/MCountGameMode.cpp

FORMS    += MainWindow.ui \
    Prefs.ui

CONFIG += mobility
MOBILITY = 

symbian {
    TARGET.UID3 = 0xe0df144b
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

OTHER_FILES += \
    debian/changelog \
    debian/compat \
    debian/control \
    debian/copyright \
    debian/README \
    debian/rules \
    qtcw.desktop

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/local/bin
    }
    INSTALLS += target
}

unix:!symbian {
    desktopfile.files = $${TARGET}.desktop
    maemo5 {
        desktopfile.path = /usr/share/applications/hildon
    } else {
        desktopfile.path = /usr/share/applications
    }
    INSTALLS += desktopfile
}
