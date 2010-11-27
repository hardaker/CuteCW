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
#include <QtGui/QIcon>
#include <QtCore/QSignalMapper>

#include "Generator.h"
#include "MorseStat.h"

#include "modes/PlayMode.h"
#include "modes/LetterTrainingMode.h"
#include "modes/SpeedTrainingMode.h"
#include "modes/WordTrainingMode.h"
#include "modes/ReadMode.h"
#include "modes/GroupingMode.h"

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

    enum TrainingMode { PLAY, TRAIN, SPEEDTRAIN, WORDS, GROUPS, READ, TEST };
    enum AudioMode { STOPPED, PLAYING, PAUSED };
    enum BadLetterWeighting { LOW = 1, HIGH = 2 };

    void setupWords();
    void setupSequences();
    void clearList();

    void createTones(int wpm);
    void createTones(float ditSecs, int dahMult = 3, int pauseMult = 1, int letterPauseMult = 3, int spaceMult = 7);

    void add(Generator *nextsound);
    void add(QChar c, bool addpause = true);
    void add(const QString &textToAdd);
    QTime addAndPlayIt(QChar c);
    QTime playIt(QChar c);

    MorseStat *getStat(const QChar &key);

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
    AudioMode audioMode();
    void setAudioMode(AudioMode newmode);
    TrainingMode trainingMode();
    BadLetterWeighting badLetterWeighting();

public slots:

    void playSequence();
    QTime maybePlaySequence();

    void generatorDone();
    void audioFinished(QAudio::State state);
    void keyPressed(QString newtext);
    void keyPressed(QChar key);

    void prefsButton();
    void switchMode(int newMode);

    void clearStatsButton();

    void playButton();

    void saveSettings();
    void loadSettings();

    void pauseAudio();
    void playAudio();

public:
    QLabel                          *m_sequenceLabel;
    QMap<QChar, QList<ditdah> *>    code;

    QMenuBar * menuBar();
private:

    QMap<TrainingMode, MorseMode *>  m_modes;

    MainWindow                      *m_parent;
    QAudioOutput                    *m_audioOutput;
    float                            m_ditSecs;
    Generator                       *m_dit, *m_dah, *m_space, *m_pause, *m_letterPause;
    Generator                       *m_playBuffer;
    AudioMode                        m_playingMode;
    TrainingMode                     m_gameMode;
    int                              m_currentWPMGoal;
    int                              m_currentWPMAccept;
    Ui::MainWindow                  *m_ui;
    BadLetterWeighting               m_badLetterWeighting;
    QTimer                           m_timer;
};

#endif // MORSE_H
