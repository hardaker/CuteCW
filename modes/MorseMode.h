#ifndef MORSEMODE_H
#define MORSEMODE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTime>
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
    MorseMode(Morse *morse, Ui::MainWindow *ui);
    Morse *morseParent();
    MorseStat *getStat(const QChar &key);

    int msToWPM(float ms);
    int msToPauseWPM(float ms);
    float msToPauseWPMF(float ms);

    void hideWidgets();

public slots:
    virtual void handleKeyPress(QChar letterPressed);
    virtual void switchToMode() = 0;
    virtual void switchToYou();

    virtual void playButton();
    virtual bool enterPressed();

    virtual void play();
    virtual void pause();
    virtual void stop();

    virtual void clear();

    virtual void audioFinished(QAudio::State state);
    virtual void audioStopped();

protected:
    Morse                          *m_morse;
    Ui::MainWindow                 *m_ui;
    int                             m_badCount, m_goodCount;
    int                             m_countWeight;
};

#endif // MORSEMODE_H
