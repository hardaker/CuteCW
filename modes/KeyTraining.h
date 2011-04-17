#ifndef KEYTRAINING_H
#define KEYTRAINING_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtCore/QSignalMapper>
#include <QtCore/QSettings>
#include <QtGui/QBoxLayout>
#include <qaudio.h>

#include "Morse.h"
#include "MorseMode.h"
#include "ui_MainWindow.h"
#include "MorseStat.h"
#include "widgets/KeyTimingDisplay.h"

class KeyTraining : public MorseMode
{
public:
    Q_OBJECT

public:
    KeyTraining(Morse *morse, Ui::MainWindow *ui);

    // should return a global name and help text for the mode:
    virtual QString name();
    virtual QString helpText();
    virtual QString icon();

    // For implementing preference screens.
    // getPrefsLayout should return a pointer to a HBox or VBox layout
    // accept/reject prefs should handle an 'ok' or 'cancel' button if needed

    // virtual QBoxLayout *getPrefsLayout();
    // virtual void acceptPrefs();
    // virtual void rejectPrefs();

    void selectKeySequence();
    void calculateStats();
    int  ditDahToMS(Morse::ditdah it);

    virtual bool enterPressed();

public slots:
    //virtual void modeMenus();             // by default does nothing; call menu() to get a mode specific menu
    // m_morse->createTones
    virtual void switchToMode();      // you can install mode widgets into m_ui->forModes

    virtual void handleKeyPress(QChar letterPressed);
    virtual void handleKeyRelease(QChar letterPressed);
    //virtual void switchFromMode();

    virtual void play();                  // called when Play is pressed
    virtual void stop();                  // called when Pause is pressed

    void changeNumberOfLetters(int value);

    // virtual void loadSettings(QSettings &settings);   // load any mode specific data
    // virtual void saveSettings(QSettings &settings);   // save any mode specific data

protected:

private:
    int        m_letterCount;
    QTime      m_startTime;
    QList<int> m_keyedTimes;
    QList<int> m_requiredTimes;
    int        m_required, m_keyCount;
    QString    m_keySequence;
    KeyTimingDisplay *m_timingDisplay;

    QLabel    *m_ditStats;
    QLabel    *m_dahStats;
    QLabel    *m_pauseStats;
    QLabel    *m_letterPauseStats;
};

#endif // KEYTRAINING_H
