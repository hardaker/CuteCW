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

#include "modes/PlayMode.h"
#include "modes/LetterTrainingMode.h"
#include "modes/SpeedTrainingMode.h"

#include "ui_MainWindow.h"
#include "ui_Prefs.h"

class MainWindow;
class PlayMode;
class MorseMode;

class Morse : public QObject
{
public:
    Q_OBJECT

public:
    Morse();
    Morse(MainWindow *parent, QAudioOutput *output, Ui::MainWindow *ui);

    enum ditdah{ DIT, DAH, SPACE, PAUSE };

    enum mode { PLAY, TRAIN, SPEEDTRAIN, WORDS, READ, TEST };
    enum playingmode { STOPPED, PLAYING, PAUSED };
    enum badLetterWeighting { LOW = 1, HIGH = 2 };
    enum sequences { KOCH = 0, KOCH1 = 1, KOCH2 = 2, KOCH3 = 3, KOCH4 = 4, ALPHABET = 5 };

    void setupWords();
    void setupSequences();
    void clearList();

    void createTones(int wpm);
    void createTones(float ditSecs, int dahMult = 3, int pauseMult = 1, int letterPauseMult = 3, int spaceMult = 7);

    void add(Generator *nextsound);
    void add(QChar c, bool addpause = true);
    void add(const QString &textToAdd);
    void addAndPlayIt(QChar c);

    MorseStat *getStat(const QChar &key);

    int  msToWPM(float ms);
    int  msToPauseWPM(float ms);
    float msToPauseWPMF(float ms);

    void setStatus(const QString &status);
    void setSequence(const QString &sequence, int currentlyAt);

    void handleKeyResponse(QChar letterPressed);
    bool enterPressed();

    Generator *dit();
    Generator *dah();
    Generator *pause();
    Generator *letterPause();
    Generator *space();

    float ditSecs();

    int currentWPMGoal();
    int currentWPMAccept();

public slots:
    void startNextTrainingKey();
    void startTimerToNextKey();

    void playSequence();
    void maybePlaySequence();

    void generatorDone();
    void audioFinished(QAudio::State state);
    void keyPressed(QString newtext);
    void keyPressed(QChar key);

    void readIt();
    void readNextLetter();

    void prefsButton();
    void switchSequence(int sequence);
    void switchMode(int newMode);

    void clearStats();
    void clearStatsButton();

    void playButton();

    void saveSettings();
    void loadSettings();

    void setDoEntireSequence(bool value);

private:

    PlayMode                        *m_playMode;

    QMap<mode, MorseMode *>          m_modes;

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
    QTextCursor                     m_readSpot;
    QMap<QChar, MorseStat *>        m_stats;
    QChar                           m_lastKey;
    QList<QChar>                    m_lastKeys;
    QList<QTime>                    m_lastTimes;
    QString                         m_trainingSequence;
    QStringList                     m_sequences;
    QLabel                          *m_statusBar;
    QLabel                          *m_sequenceLabel;
    Ui::MainWindow                  *m_ui;
    badLetterWeighting               m_badLetterWeighting;
    QSignalMapper                   *m_sequenceSignalMapper;
    QTimer                           m_timer;
    int                              m_countWeight;
    int                              m_badCount, m_goodCount;
    bool                             m_doEntireSequence;
};

#endif // MORSE_H
