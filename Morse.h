#ifndef MORSE_H
#define MORSE_H

#include <QtCore/QString>
#include <QtMultimedia/QAudioOutput>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtGui/QLabel>
#include <QtCore/QTime>

#include "Generator.h"
#include "MorseStat.h"

#define KOCH_GROUP "kmuresnaptlwi.jz=foy,vg5/q92h38b?47c1d60x"

class Morse : public QObject
{
public:
    Q_OBJECT

public:
    Morse();
    Morse(QAudioOutput *output, QLabel *statusBar, QLabel *sequence);

    enum ditdah{ DIT, DAH };

    enum mode { PLAY, TRAIN, TEST };
    enum playingmode { STOPPED, PLAYING };

    void clearList();
    void add(Generator *nextsound);
    void add(QChar c, bool addpause = true);
    void addAndPlayIt(QChar c);
    void createTones(float ditSecs, int dahMult = 3, int pauseMult = 1, int letterPauseMult = 3, int spaceMult = 7);
    MorseStat *getStat(const QChar &key);
    void startNextTrainingKey();
    int  msToWPM(float ms);

    void setStatus(const QString &status);
    void setSequence(const QString &sequence, int currentlyAt);

    Generator *dit();
    Generator *dah();
    Generator *pause();
    Generator *letterPause();
    Generator *space();

public slots:
    void playSequence();
    void maybePlaySequence();
    void audioFinished(QAudio::State state);
    void keyPressed(QString newtext);
    void switchMode(int newMode);

private:
    QAudioOutput                    *m_audioOutput;
    float                           m_ditSecs;
    Generator                       *m_dit, *m_dah, *m_space, *m_pause, *m_letterPause;
    Generator                       *m_playBuffer;
    QMap<QChar, QList<ditdah> *>    code;
    playingmode                     m_playingMode;
    mode                            m_gameMode;
    int                             m_currentWPMGoal;
    QMap<QChar, MorseStat *>        m_stats;
    QLabel                          *m_statusBar;
    QChar                           m_lastKey;
    QTime                           m_lastTime;
    QString                         m_trainingSequence;
    QLabel                          *m_sequenceLabel;

};

#endif // MORSE_H
