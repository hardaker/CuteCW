#-------------------------------------------------
#
# Project created by QtCreator 2010-07-01T21:21:28
#
#-------------------------------------------------

QT       += core gui multimedia network xml

TARGET = cutecw
TEMPLATE = app

RESOURCES = cutecw.qrc

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
    modes/KeyTraining.cpp \
    modes/games/MGameMode.cpp \
    modes/games/MCountGameMode.cpp \
    modes/games/MTimeGameMode.cpp \
    modes/games/GroupGame.cpp \
    modes/games/WordGame.cpp \
    modes/games/HighScoresDialog.cpp \
    modes/games/HighScores.cpp \
    modes/QModeStart.cpp \
    AboutDialog.cpp \
    modes/FrontPage.cpp \
    GoalBar.cpp \
    widgets/KeyTimingDisplay.cpp

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
    modes/KeyTraining.h \
    modes/games/MGameMode.h \
    modes/games/MCountGameMode.h \
    modes/games/MTimeGameMode.h \
    modes/games/GroupGame.h \
    modes/games/WordGame.h \
    modes/games/HighScoresDialog.h \
    modes/games/HighScores.h \
    modes/QModeStart.h \
    AboutDialog.h \
    modes/FrontPage.h \
    GoalBar.h \
    widgets/KeyTimingDisplay.h

FORMS    += AboutDialog.ui

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
    cutecw.desktop

BINDIR = $$PREFIX/bin
DATADIR =$$PREFIX/share

macx {
    CONFIG += x86 x86_64
    #  CONFIG += x86 ppc x86_64 ppc64
}

unix:!symbian {
    icon26.files = icons/26x26/cutecw.png
    icon48.files = icons/48x48/cutecw.png
    icon64.files = icons/64x64/cutecw.png

    icon26.path = $$DATADIR/icons/hicolor/26x26/apps
    icon48.path = $$DATADIR/icons/hicolor/48x48/apps
    icon64.path = $$DATADIR/icons/hicolor/64x64/apps

    desktopfile.files = $${TARGET}.desktop
    maemo5 {
        target.path = /opt/usr/bin
        desktopfile.path = /usr/share/applications/hildon
    } else {
        target.path = /usr/local/bin
        desktopfile.path = /usr/share/applications
    }

    INSTALLS += desktopfile
    INSTALLS += target
    INSTALLS += icon26 icon48 icon64
}
