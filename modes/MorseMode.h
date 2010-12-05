#ifndef MORSEMODE_H
#define MORSEMODE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTime>
#include <QtCore/QSignalMapper>
#include <qaudio.h>

// #include "Morse.h"
#include "ui_MainWindow.h"
#include "MorseStat.h"

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

    int msToWPM(float ms);
    int msToPauseWPM(float ms);
    float msToPauseWPMF(float ms);

    void hideWidgets();
    void setupKeyWidgets(const QString &sequence);
    void createGlobalActions();

    RunningMode runningMode();
    void setRunningMode(RunningMode newMode);

    void clearLayout(QLayout *layout);

public slots:
    virtual void handleKeyPress(QChar letterPressed); // by default does nothing
    virtual void handleKeyPress(const QString &letterPressed);  // by default calls the QChar version
    virtual void switchToMode() = 0;
    virtual void switchFromMode();
    virtual void switchToYou();
    virtual void switchFromYou();

    virtual void playButton();
    virtual bool enterPressed();

    virtual void play();
    virtual void pause();
    virtual void stop();

    virtual void clear();
    virtual void clearModeLayout();

    virtual void help();

    virtual void audioFinished(QAudio::State state);
    virtual void audioStopped();

protected:
    Morse                          *m_morse;
    Ui::MainWindow                 *m_ui;
    int                             m_badCount, m_goodCount;
    int                             m_countWeight;
    QIcon                           m_playIcon, m_pauseIcon;
    RunningMode                     m_runningMode;
    QSignalMapper                  *m_mapper;
    QGridLayout                    *m_buttons;
};

#endif // MORSEMODE_H
