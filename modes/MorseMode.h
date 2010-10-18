#ifndef MORSEMODE_H
#define MORSEMODE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTime>
#include <QAudio>

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
    void handleKeyResponse(QChar letterPressed);

public slots:
    virtual void handleKeyPress(QChar letterPressed) = 0;
    virtual void switchToMode() = 0;

    virtual void playButton();
    virtual bool enterPressed();

    virtual void play();
    virtual void pause();

    virtual void clear();

    virtual void audioFinished(QAudio::State state);

protected:
    Morse                          *m_morse;
    Ui::MainWindow                 *m_ui;
    int                             m_badCount, m_goodCount;
    QList<QChar>                    m_lastKeys;
    QList<QTime>                    m_lastTimes;
    QMap<QChar, MorseStat *>        m_stats;
    int                             m_countWeight;
};

#endif // MORSEMODE_H
