#ifndef MORSEMODE_H
#define MORSEMODE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtCore/QSignalMapper>
#include <QtCore/QSettings>
#include <QtGui/QBoxLayout>
#include <qaudio.h>

// #include "Morse.h"
#include "ui_MainWindow.h"
#include "MorseStat.h"

#if (defined(Q_WS_MAEMO_5) || defined(MAEMO_CHANGES))
#define SMALL_DEVICE 1
#endif

#ifdef Q_OS_WIN32
// cross compiling pulls in the wrong stdlib.h for some reason
#undef RAND_MAX
#define RAND_MAX 0x7FFF
#endif

class Morse;

class MorseMode : public QObject
{
public:
    Q_OBJECT

public:
    enum RunningMode { RUNNING = 0, PAUSED = 1 };

    MorseMode(Morse *morse, Ui::MainWindow *ui);
    Morse *morseParent();
    MorseStat *getStat(const QChar &key);

    // converts time delays into WPM rates:
    int msToWPM(float ms);
    int msToPauseWPM(float ms);
    float msToPauseWPMF(float ms);

    void setupKeyWidgets(const QString &sequence, QBoxLayout *inside = 0);
    void setupWPMWidgets(QBoxLayout *to = 0);

    // sets up the sequence: and related bottom bars
    virtual void setupSequenceLayout(QVBoxLayout *parentLayout = 0,
                                      QWidget *theMainThing = 0);
    virtual void setupLastWPMLayout(QVBoxLayout *parentLayout = 0,
                                      QWidget *theMainThing = 0);
    virtual void setupAveWPMLayout(QVBoxLayout *parentLayout = 0,
                                      QWidget *theMainThing = 0);
    virtual void setupSequenceLayouts(QVBoxLayout *parentLayout = 0,
                                      QWidget *theMainThing = 0);
#ifdef SMALL_DEVICE
    virtual QMenuBar *menu();
#else
    virtual QMenu *menu();
#endif
    RunningMode runningMode();
    void setRunningMode(RunningMode newMode);

    // erases the contents of a layout object
    void clearLayout(QLayout *layout);

    // should return a global name and help text for the mode:
    virtual QString name() = 0;
    virtual QString helpText() = 0;
    virtual QString icon();

    // For implementing preference screens.
    // getPrefsLayout should return a pointer to a HBox or VBox layout
    // accept/reject prefs should handle an 'ok' or 'cancel' button if needed
    virtual QBoxLayout *getPrefsLayout();
    virtual void acceptPrefs();
    virtual void rejectPrefs();

public slots:
    // switchToYou, by default calls the following ones in order:
    virtual void switchToYou();
    virtual void createMenuStructures();  // this calls modeMenus():
    virtual void modeMenus();             // by default does nothing; call menu() to get a mode specific menu
    // m_morse->createTones
    virtual void hideWidgets();
    virtual void switchToMode() = 0;      // you can install mode widgets into m_ui->forModes
    virtual void createGlobalActions();

    virtual void handleKeyPress(const QString &letterPressed);  // by default calls the QChar version:
    virtual void handleKeyPress(QChar letterPressed); // by default does nothing
    virtual void switchFromMode();
    virtual void switchFromYou();

    // by default this changes the mode to RUNNING or PAUSED (and stops the audio)
    virtual void playButton();            // sets the icons, and calls play() or stop()
    virtual bool enterPressed();

    virtual void play();                  // called when Play is pressed
    virtual void stop();                  // called when Pause is pressed

    virtual void clear();
    virtual void clearModeLayout();      // calls clearLayout on m_ui->forModes

    virtual void help();                 // opens a help window to show helpText() contents

    virtual void audioFinished(QAudio::State state);  // changes the internal state and calls:
    virtual void audioStopped();                      // this when audio has stopped playing

    virtual void changeWPM(int wpm);                  // call this to change the WPM rate
    virtual void changeSpaceWPM(int wpm);             // call this to change the spacing WPM rate
    virtual void changeLetterSpaceWPM(int wpm);       // call this to change the spacing between letters

    virtual void loadSettings(QSettings &settings);   // load any mode specific data
    virtual void saveSettings(QSettings &settings);   // save any mode specific data

protected:
    Morse                          *m_morse;
    Ui::MainWindow                 *m_ui;
    int                             m_badCount, m_goodCount;
    int                             m_WPM, m_spaceWPM, m_letterSpaceWPM;
    QIcon                           m_playIcon, m_pauseIcon;
    RunningMode                     m_runningMode;
    QSignalMapper                  *m_mapper;
    QGridLayout                    *m_buttons;
#ifdef SMALL_DEVICE
    // on small devices (ie, maemo) everything goes straight into the main help bar
    QMenuBar                       *m_helpMenu;
    QMenuBar                       *m_optionsMenu;
    QMenuBar                       *m_modeSpecificMenu;
    QMenuBar                       *m_cuteCWMenu;
#else
    QMenu                          *m_helpMenu;
    QMenu                          *m_optionsMenu;
    QMenu                          *m_modeMenu;
    QMenu                          *m_modeSpecificMenu;
    QMenu                          *m_cuteCWMenu;
#endif

    QHBoxLayout                    *m_sequenceLayout;
    QHBoxLayout                    *m_lastWPMLayout;
    QHBoxLayout                    *m_aveWPMLayout;

    QLabel                         *m_avewpmLabel;
    QLabel                         *m_lastwpmLabel;
    QLabel                         *m_sequenceLabel;
};

#endif // MORSEMODE_H
