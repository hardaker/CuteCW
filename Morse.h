#ifndef MORSE_H
#define MORSE_H

#include <QtCore/QString>
#include <QtMultimedia/QAudioOutput>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtGui/QLabel>
#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QtGui/QTextCursor>
#include <QtCore/QSignalMapper>

#include "Generator.h"
#include "MorseStat.h"

#include "ui_MainWindow.h"
#include "ui_Prefs.h"

class MainWindow;

class Morse : public QObject
{
public:
    Q_OBJECT

public:
    Morse();
    Morse(MainWindow *parent, QAudioOutput *output, Ui::MainWindow *ui);

    enum ditdah{ DIT, DAH, SPACE, PAUSE };

    enum mode { PLAY, TRAIN, SPEEDTRAIN, READ, TEST };
    enum playingmode { STOPPED, PLAYING };
    enum badLetterWeighting { LOW = 1, HIGH = 2 };
    enum sequences { KOCH = 0, KOCH1 = 1, KOCH2 = 2, KOCH3 = 3, KOCH4 = 4, ALPHABET = 5 };

    void setupSequences();
    void clearList();
    void add(Generator *nextsound);
    void add(QChar c, bool addpause = true);
    void add(const QString &textToAdd);
    void addAndPlayIt(QChar c);
    void createTones(int wpm);
    void createTones(float ditSecs, int dahMult = 3, int pauseMult = 1, int letterPauseMult = 3, int spaceMult = 7);
    MorseStat *getStat(const QChar &key);
    int  msToWPM(float ms);
    int  msToPauseWPM(float ms);
    void startTimerToNextKey();

    void setStatus(const QString &status);
    void setSequence(const QString &sequence, int currentlyAt);

    void handleKeyResponse(QChar letterPressed);

    Generator *dit();
    Generator *dah();
    Generator *pause();
    Generator *letterPause();
    Generator *space();

public slots:
    void startNextTrainingKey();
    void playSequence();
    void maybePlaySequence();
    void audioFinished(QAudio::State state);
    void keyPressed(QString newtext);
    void keyPressed(QChar key);
    void switchMode(int newMode);
    void prefsButton();
    void readIt();
    void readNextLetter();
    void switchSequence(int sequence);
    void clearStats();
    void clearStatsButton();

    void saveSettings();
    void loadSettings();

private:
    MainWindow                      *m_parent;
    QAudioOutput                    *m_audioOutput;
    float                           m_ditSecs;
    Generator                       *m_dit, *m_dah, *m_space, *m_pause, *m_letterPause;
    Generator                       *m_playBuffer;
    QMap<QChar, QList<ditdah> *>    code;
    playingmode                     m_playingMode;
    mode                            m_gameMode;
    int                             m_currentWPMGoal;
    int                             m_currentWPMAccept;
    int                             m_badCount, m_goodCount;
    QTextCursor                     m_readSpot;
    QMap<QChar, MorseStat *>        m_stats;
    QChar                           m_lastKey;
    QList<QChar>                    m_lastKeys;
    QTime                           m_lastTime;
    QString                         m_trainingSequence;
    QStringList                     m_sequences;
    QLabel                          *m_statusBar;
    QLabel                          *m_sequenceLabel;
    Ui::MainWindow                  *m_ui;
    badLetterWeighting               m_badLetterWeighting;
    QSignalMapper                   *m_signalMapper;
    QTimer                           m_timer;
    int                              m_countWeight;
};

#endif // MORSE_H
