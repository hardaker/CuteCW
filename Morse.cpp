#include "Morse.h"

#include <QtCore/QSettings>
#include <QtGui/QMenu>
#include <qdebug.h>

#include "MainWindow.h"

#define WPMGOAL   20
#define WPMACCEPT 15

Morse::Morse()
    : QObject(), m_parent(0), m_audioOutput(), m_dit(0), m_dah(0), m_space(0), m_pause(0), m_letterPause(0), m_playingMode(STOPPED), m_gameMode(PLAY),
    m_currentWPMGoal(WPMGOAL), m_currentWPMAccept(WPMACCEPT), m_statusBar(0), m_sequenceLabel(0), m_ui(0), m_countWeight(50), m_badCount(0), m_goodCount(0),
    m_doEntireSequence(false)
{
    qDebug() << "new morse";
    m_modes.insert(PLAY, new PlayMode(this));
    setupSequences();
}

Morse::Morse(MainWindow *parent, QAudioOutput *output, Ui::MainWindow *ui)
    : QObject(parent), m_parent(parent), m_audioOutput(output), m_dit(0), m_dah(0), m_space(0), m_pause(0), m_letterPause(0), m_playingMode(STOPPED), m_gameMode(PLAY),
    m_currentWPMGoal(WPMGOAL), m_currentWPMAccept(WPMACCEPT),
    m_statusBar(ui->status), m_sequenceLabel(ui->sequence), m_ui(ui), m_countWeight(100), m_badCount(0), m_goodCount(0)
{

    qDebug() << "new morse2";
    createTones(WPMGOAL);
    setStatus("ready: Play Mode");
    qsrand(QTime::currentTime().msec());
    loadSettings();
    switchMode(Morse::PLAY);

    connect(m_ui->readButton, SIGNAL(clicked()), this, SLOT(readIt()));
    connect(m_ui->clearTraining, SIGNAL(clicked()), this, SLOT(clearStatsButton()));
    connect(m_ui->play, SIGNAL(clicked()), this, SLOT(playButton()));

    setupSequences();
    setupWords();
}

void Morse::setupWords() {

    m_wordsNumber = Morse::N100;

#include "words/100words.h"
#include "words/200words.h"
#include "words/300words.h"
#include "words/400words.h"
#include "words/500words.h"

    m_wordSignalMapper = new QSignalMapper();
    QMenu *modeMenu = new QMenu(m_ui->changeWords);
    m_ui->changeWords->setMenu(modeMenu);

    QAction *action = modeMenu->addAction("Words 1-100");
    connect(action, SIGNAL(triggered()), m_wordSignalMapper, SLOT(map()));
    m_wordSignalMapper->setMapping(action, (int) Morse::N100);

    action = modeMenu->addAction("Words 101-200");
    connect(action, SIGNAL(triggered()), m_wordSignalMapper, SLOT(map()));
    m_wordSignalMapper->setMapping(action, (int) Morse::N200);

    action = modeMenu->addAction("Words 201-300");
    connect(action, SIGNAL(triggered()), m_wordSignalMapper, SLOT(map()));
    m_wordSignalMapper->setMapping(action, (int) Morse::N300);

    action = modeMenu->addAction("Words 301-400");
    connect(action, SIGNAL(triggered()), m_wordSignalMapper, SLOT(map()));
    m_wordSignalMapper->setMapping(action, (int) Morse::N400);

    action = modeMenu->addAction("Words 401-500");
    connect(action, SIGNAL(triggered()), m_wordSignalMapper, SLOT(map()));
    m_wordSignalMapper->setMapping(action, (int) Morse::N500);

    connect(m_wordSignalMapper, SIGNAL(mapped(int)), this, SLOT(switchWords(int)));
}

void Morse::switchWords(int sequence) {
    m_wordsNumber = (wordNums) sequence;
}

void Morse::startNextWord() {
    m_wordnumber = qrand()%(m_maxWord);
    add((*(words[m_wordsNumber]))[m_wordnumber]);
    maybePlaySequence();
    m_enteredWord = "";
    m_ui->letter->setText("");
    m_wordWasGood = true;
}

void Morse::handleWordResponse(QChar letter) {
    if (letter == '\r') {
        startNextWord();
        return;
    }
    if ((*(words[m_wordsNumber]))[m_wordnumber][m_enteredWord.length()] == letter) {
        m_ui->letter->setText(m_ui->letter->text() + "<font color=\"green\">" + letter + "<font>");
    } else {
        m_ui->letter->setText(m_ui->letter->text() + "<font color=\"red\">" + letter + "<font>");
        m_wordWasGood = false;
    }
    m_enteredWord.append(letter);
    if ((*(words[m_wordsNumber]))[m_wordnumber].length() == m_enteredWord.length()) {
        if (m_wordWasGood) {
            m_ui->letter->setText(m_ui->letter->text() + " - <font color=\"green\">GOOD</font>");
            m_maxWord += 2;
            if (m_maxWord > (*(words[m_wordsNumber])).count())
                m_maxWord = (*(words[m_wordsNumber])).count();
        } else {
            m_ui->letter->setText(m_ui->letter->text() + " - <font color=\"red\">FAIL (" + (*(words[m_wordsNumber]))[m_wordnumber] + ")</font>");
            if (m_maxWord > 1)
                m_maxWord--;
        }
    }
}

void Morse::setupSequences() {
    m_trainingSequence = "kmuresnaptlwi.jz=foy,vg5/q92h38b?47c1d60x";
    m_sequences.append("kmuresnaptlwi.jz=foy,vg5/q92h38b?47c1d60x");
    m_sequences.append("kmuresnaptl");
    m_sequences.append("wi.jz=foy,");
    m_sequences.append("vg5/q92h38");
    m_sequences.append("b?47c1d60x");
    m_sequences.append("abcdefghijklmnopqrstuvwxyz1234567890.,/=?");


    m_sequenceSignalMapper = new QSignalMapper();
    QMenu *modeMenu = new QMenu(m_ui->changeSequence);
    m_ui->changeSequence->setMenu(modeMenu);

    QAction *action = modeMenu->addAction("Full Koch");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) Morse::KOCH);

    action = modeMenu->addAction("Koch part 1");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) Morse::KOCH1);

    action = modeMenu->addAction("Koch part 2");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) Morse::KOCH2);

    action = modeMenu->addAction("Koch part 3");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) Morse::KOCH3);

    action = modeMenu->addAction("Koch part 4");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) Morse::KOCH4);

    action = modeMenu->addAction("Alphabet");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) Morse::ALPHABET);

    connect(m_sequenceSignalMapper, SIGNAL(mapped(int)), this, SLOT(switchSequence(int)));
}

void Morse::prefsButton() {
    Ui::Prefs prefsDialog;
    QDialog *dialog = new QDialog(m_parent);
    prefsDialog.setupUi(dialog);
    prefsDialog.WPMAccepted->setText(QString().setNum(m_currentWPMAccept));
    prefsDialog.WPMGoal->setText(QString().setNum(m_currentWPMGoal));

    prefsDialog.weighting->insertItem(LOW, "Low");
    prefsDialog.weighting->insertItem(HIGH, "High");
    prefsDialog.weighting->setCurrentIndex(m_badLetterWeighting);

    if (dialog->exec() == QDialog::Accepted) {
        m_currentWPMAccept = prefsDialog.WPMAccepted->text().toInt();
        m_currentWPMGoal = prefsDialog.WPMGoal->text().toInt();
        m_badLetterWeighting = (badLetterWeighting) prefsDialog.weighting->currentIndex();
        saveSettings();
        loadSettings();
    }
}

void Morse::saveSettings() {
    QSettings settings("WS6Z", "qtcw");
    settings.setValue("WPM/Goal", m_currentWPMGoal);
    settings.setValue("WPM/Accept", m_currentWPMAccept);
    settings.setValue("LetterWeighting", int(m_badLetterWeighting));
    qDebug() << "saving: " << m_badLetterWeighting;
}

void Morse::loadSettings() {
    QSettings settings("WS6Z", "qtcw");
    m_currentWPMGoal = settings.value("WPM/Goal", WPMGOAL).toInt();
    m_currentWPMAccept = settings.value("WPM/Accept", WPMACCEPT).toInt();
    m_badLetterWeighting = (badLetterWeighting) settings.value("LetterWeighting", HIGH).toInt();
    createTones(m_currentWPMGoal);  
}

void Morse::clearStats()  {
    QMap<QChar, MorseStat *>::iterator item;
    QMap<QChar, MorseStat *>::iterator end = m_stats.end();
    for(item = m_stats.begin(); item != end; ++item) {
        delete *item;
    }
    m_stats.clear();
    m_lastKeys.clear();
    m_lastTimes.clear();
}

void Morse::clearStatsButton() {
    clearStats();
    startNextTrainingKey();
    m_goodCount = 0;
    m_badCount = 0;
}

void
Morse::playSequence()
{
    qDebug() << "Playing!";
    m_playBuffer->restartData();
    m_playBuffer->start();
    m_playingMode = PLAYING;
    m_audioOutput->start(m_playBuffer);
    return;
}

void Morse::readIt() {
    QTextCursor readSpot = m_ui->wordbox->textCursor();
    readSpot.select(QTextCursor::Document);
    add(readSpot.selectedText());
    playSequence();
    return;
    qDebug() << "starting to read";
    m_readSpot = m_ui->wordbox->cursorForPosition(QPoint(0,0));
    readNextLetter();
}

void Morse::readNextLetter() {
    if (m_readSpot.atEnd()) {
        qDebug() << "reached the end; stopping playing";
        return;
    }
    // move the anchor to where the cursor is
    m_readSpot.movePosition(QTextCursor::NoMove, QTextCursor::MoveAnchor, 0);
    // move the selection pointer to the right one, highlighting the current
    // selection letter we're going to play
    m_readSpot.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
    // play the selection
    qDebug() << "playing selected text: " << m_readSpot.selectedText();
    addAndPlayIt(m_readSpot.selectedText()[0]);
}

void Morse::maybePlaySequence() {
    qDebug() << "gmaybe oing to key: " << " / " << m_playingMode;
    if (m_playingMode == STOPPED || m_playingMode == PAUSED) {
        qDebug() << "going to key: ";
        playSequence();
    }
}

void Morse::addAndPlayIt(QChar c) {
    if (m_playingMode == STOPPED || m_playingMode == PAUSED) {
        clearList();
        add(pause());
    }
    add(c, false);
    add(m_letterPause);
    maybePlaySequence();
}

void Morse::keyPressed(QString newtext) {
    QChar newletter = newtext.at(newtext.length()-1).toLower();
    qDebug() << "user pressed: " << newletter << "and last key was: " << m_lastKey;
    keyPressed(newletter);
}

bool Morse::enterPressed() {
    if (m_gameMode != WORDS)
        return false;
    startNextWord();
    return true;
}

void Morse::playButton() {
    if (m_playingMode == PAUSED) {
        m_playingMode = STOPPED;
        m_lastKeys.clear();
        m_lastTimes.clear();
        m_ui->play->setText("Pause");

        switch (m_gameMode) {
        case PLAY:
            break;
        case TRAIN:
        case SPEEDTRAIN:
            startNextTrainingKey();
            break;
        case WORDS:
            startNextWord();
            break;
        case READ:
            // XXX
            break;
        case TEST:
            break;
        }

        // XXX: then do something based on the mode
    } else { // PLAYING or PAUSED
        m_playingMode = PAUSED;
        m_ui->play->setText("Go");

        switch (m_gameMode) {
        case PLAY:
            break;
        case TRAIN:
        case SPEEDTRAIN:
            break;
        case WORDS:
            break;
        case READ:
            // XXX
            break;
        case TEST:
            break;
        }

    }
}

void Morse::handleKeyResponse(QChar letterPressed) {

    qDebug() << "Key pressed = " << letterPressed << ", Queue of stored keys: keys=" << m_lastKeys.count() << ", times=" << m_lastTimes.count();

    // ensure we actually have a stored key in memory
    if (m_lastTimes.count() == 0) {
        qDebug() << "KEY PRESSED EARLY";
        return;
    }

    // pull off the last key from the "keyed" output
    QChar lastKey = m_lastKeys.takeFirst();
    QTime lastTime = m_lastTimes.takeFirst();

    // calculate the time since the keying ended to the time the user hit a key
    // XXX: we need to store a list of times, not just a single time

    int msElapsed = lastTime.elapsed() - m_ditSecs; // subtract off blank-after time
    if (msElapsed <= 0)
        msElapsed = 1;
    qDebug() << "Training response: elapsed " << msElapsed << "ms (" << msToPauseWPM(msElapsed) << " WPM)";
    MorseStat *pressedStat = getStat(letterPressed);

    // if the user took a *really* long time, ignore the key press and assume they got distracted from training
    if ((pressedStat->getTryCount() > 0 && msElapsed > 5 * pressedStat->getAverageTime()) ||
        (pressedStat->getTryCount() == 0 && msElapsed > 5 * msToPauseWPM(m_currentWPMAccept))) {
        qDebug() << "ignoring key press; too long and probably an interruption";
        return;
    }

    // set the last WPM record on the display
    m_ui->lastwpm->setText(QString().setNum(msToPauseWPM(msElapsed)));

    // if the keyed incorrectly, penalize them 3 times their average else add in the results
    if (letterPressed == lastKey) {
        pressedStat->addTime(msElapsed);
        m_goodCount++;
    } else {
        if (pressedStat->getAverageTime() > 0) /* don't do this unless the letter has been pressed before */
            pressedStat->addTime(3.0 * pressedStat->getAverageTime());
        if (getStat(lastKey)->getAverageTime() > 0) /* don't do this unless the letter has been pressed before */
            getStat(lastKey)->addTime(3.0 * getStat(lastKey)->getAverageTime());
        m_badCount++;
    }
}

void Morse::keyPressed(QChar newletter) {
    switch (m_gameMode) {
    case PLAY:
        addAndPlayIt(newletter);
        break;
    case TRAIN:
        // ensure we're not still playing a sound:
        if (m_playingMode == PLAYING)
            return;
        // analyze they're keyed letter and immediately start playing a new one
        handleKeyResponse(newletter);
	startNextTrainingKey();
        break;
    case SPEEDTRAIN:
        handleKeyResponse(newletter);
        break;
    case WORDS:
        handleWordResponse(newletter);
        break;
    default:
        qDebug() << "ignoring key: " << newletter;
    }
}

int Morse::msToWPM(float ms) {
    return (60*1000)/ms; // XXX: fix me; doesn't include keying times
}

int Morse::msToPauseWPM(float ms) {
    return int(msToPauseWPMF(ms));
}

float Morse::msToPauseWPMF(float ms) {
    // 3 dits in length is the pause between letter spacing
    float pauseLength = 3.0 * m_ditSecs;
    // calculate the WPM based on the space it took for the letter to be identified during the pause
    // qDebug() << "pause length: " << pauseLength << ", recorded time: " << ms << ", % = " << (pauseLength * 1000.0 * 100.0 / ms );
    return float(m_currentWPMGoal) * pauseLength * 1000.0 / ms;
}

void Morse::startNextTrainingKey() {
    qDebug() << "--- Start next training key";
    int letterCount = 0;
    QList<QPair<QChar, float> > letters;
    float totalTime = 0.0, thisTime = 0.0, minTime = 0.0;
    MorseStat *stat = 0;
    QString currentLetterGoal;

    if (m_playingMode == PAUSED)
        return;

    QString::iterator letter;
    QString::iterator lastLetter = m_trainingSequence.end();
    for(letter = m_trainingSequence.begin(); letter != lastLetter; ++letter) {
        letterCount++;
        currentLetterGoal = (*letter).toUpper();
        stat = getStat(*letter);
        thisTime = stat->getAverageTime();
        totalTime += thisTime;
        if (minTime > thisTime)
            minTime = thisTime;
        if (thisTime < 0) {
            if (m_doEntireSequence) {
                thisTime = 1000*60.0/(50.0*float(m_currentWPMAccept));
            } else {
                // never keyed yet; do it immediately if we got this far
                setStatus("Starting a new letter: " + QString(*letter));
                qDebug() << "|------ keying: " << *letter;
                m_lastKey = *letter;
                m_lastKeys.append(*letter);
                setSequence(m_trainingSequence, letterCount);
                m_ui->avewpm->setText("All WPM: " + QString().setNum(msToPauseWPM(totalTime/letterCount)) + ", " +
                                      *letter + ": " + QString().setNum(msToPauseWPM(thisTime)));
                if (m_gameMode == SPEEDTRAIN)
                    m_ui->WPM->setText(QString().setNum(msToPauseWPMF((float(m_badCount + m_countWeight)/float(m_goodCount + m_countWeight)) *
                                                                      totalTime/float(letterCount)), 'g', 2));
                else
                    m_ui->WPM->setText(QString().setNum(msToPauseWPMF(totalTime/float(letterCount)), 'g', 2));
                addAndPlayIt(*letter);
                return;
            }
        }

        qDebug() << "  adding " << *letter << " / " << thisTime << " / " << msToPauseWPM(thisTime);
        letters.append(QPair<QChar, float>(*letter, thisTime));

        if(msToPauseWPM(thisTime) <= m_currentWPMAccept) {
            // we're not fast enough; break here
            qDebug() << " too slow: " << *letter << " / " << thisTime << " / " << msToPauseWPM(thisTime);
            break;
        }
    }

    // They have the whole sequence active at this point

    m_ui->avewpm->setText("All WPM: " + QString().setNum(msToPauseWPM(totalTime/letterCount)) + ", " +
                          currentLetterGoal + " WPM: " + QString().setNum(msToPauseWPM(thisTime/stat->getTryCount())));
    if (m_gameMode == SPEEDTRAIN)
        m_ui->WPM->setText(QString().setNum(msToPauseWPMF((float(m_badCount + m_countWeight)/float(m_goodCount + m_countWeight)) * totalTime/float(letterCount))));
    else
        m_ui->WPM->setText(QString().setNum(msToPauseWPMF(totalTime/float(letterCount))));
    // now pick a random time between 0 and the total of all the averages; averages with a slower speed are more likely
    // XXX: probably could use a weighted average (subtract off min speed from all speeds)?
    
    float randTime, subTime = 0.0;
    if (m_badLetterWeighting == HIGH) {
        subTime = minTime/2;
        randTime = (totalTime - subTime * letters.count())*float(qrand())/float(RAND_MAX);
    } else
        randTime = totalTime*float(qrand())/float(RAND_MAX);
    float newTotal = 0;
    qDebug() << "letter set random: " << randTime << " total: " << totalTime << " min: " << minTime/2 << ", count: " << letters.count();
    QList<QPair<QChar, float> >::iterator search;
    QList<QPair<QChar, float> >::iterator last = letters.end();
    setSequence(m_trainingSequence, letterCount);
    for(search = letters.begin(); search != last; ++search) {
        //qDebug() << "  -> " << (*search).first << "/" << (*search).second;
        newTotal += ((*search).second - subTime);
        if (newTotal > randTime) {
            qDebug() << "------- keying: " << (*search).first;
            m_lastKey = (*search).first;
            m_lastKeys.append((*search).first);
            addAndPlayIt((*search).first);
            return;
        }
    }
    qDebug() << "**** shouldn't get here: " << randTime << "," << totalTime;
}

MorseStat *Morse::getStat(const QChar &key) {
    if (! m_stats.contains(key))
        m_stats[key] = new MorseStat(0);
    return m_stats[key];
}

void Morse::startTimerToNextKey() {
    float avetime, delay;

    if (m_lastTimes.count() > 3) {
        // don't let them get *too* far behind
        m_badCount++;
        QTimer::singleShot(1000, this, SLOT(startNextTrainingKey()));  // Try again in a second
        return;
    }
    avetime = getStat(m_lastKey)->getAverageTime();
    qDebug() << "avetime: " << avetime;
    if (avetime == -1) {
        avetime = 1000*60.0/(50.0*float(m_currentWPMAccept));
        qDebug() << "setting avetime to: " << avetime;
    }
    delay  = (float(m_badCount + m_countWeight)/float(m_goodCount + m_countWeight)) * avetime;
    qDebug() << "delaying for: " << delay << " ms (good=" << m_goodCount << ", bad=" << m_badCount << ")";
    QTimer::singleShot(delay, this, SLOT(startNextTrainingKey()));
}

void Morse::generatorDone() {
    //qDebug() << "generator says done";
    audioFinished(QAudio::StoppedState); // fixes windows issues
}

void
Morse::audioFinished(QAudio::State state)
{
    //qDebug() << "audio state changed: " << state;
    if (state != QAudio::IdleState && state != QAudio::StoppedState)
        return;
    switch (m_gameMode) {
    // case READ:
        // add in next letter and display it
        // readNextLetter();
        // break;

    case SPEEDTRAIN:
        qDebug() << "speed train stop";
        if (m_playingMode != STOPPED) {
            startTimerToNextKey();
            m_lastTimes.push_back(QTime::currentTime());
        }
        m_playingMode = STOPPED;
        break;

    case WORDS:
        qDebug() << "words train stop";
        if (m_playingMode != STOPPED) {
            m_lastTimes.push_back(QTime::currentTime());
        }
        m_playingMode = STOPPED;
        break;

    default:
        if (m_playingMode != STOPPED) {
            m_lastTimes.push_back(QTime::currentTime());
        }
        m_playingMode = STOPPED;
    }
}

void
Morse::clearList()
{
    m_playBuffer->clearBuffer();
}

void
Morse::add(Generator *nextSound)
{
    m_playBuffer->appendDataFrom(nextSound);
}

void Morse::switchMode(int newmode) {
    m_gameMode = (Morse::mode) newmode;
    qDebug() << "switch to:" << m_gameMode;
    m_lastKeys.clear();
    m_lastTimes.clear();
    m_playBuffer->stop();
    m_ui->letter->setText("");
    m_ui->WPM->setText("");
    switch (m_gameMode) {
    case PLAY:
        m_ui->wordbox->hide();
        m_ui->letter->hide();
        m_ui->clearTraining->hide();
        m_ui->readButton->hide();
        m_ui->changeSequence->hide();
        m_ui->changeWords->hide();
        m_ui->modeMenu->setText("Play Morse Code");
        m_ui->helpBar->setText("<font color=\"green\">Type letters to hear the keys in morse code</font>");
        m_ui->play->hide();
        m_ui->WPM->hide();
        break;
    case TRAIN:
        m_ui->wordbox->hide();
        m_ui->letter->show();
        m_ui->clearTraining->show();
        m_ui->readButton->hide();
        m_ui->modeMenu->setText("Recognition Training");
        m_ui->changeSequence->show();
        m_ui->changeWords->hide();
        m_ui->helpBar->setText("<font color=\"green\">Type the letter you hear ASAP.</font>");
        m_ui->play->show();
        m_ui->WPM->show();
        m_playingMode = PLAYING;
        playButton(); // will change to "paused"
        startNextTrainingKey();
        break;
    case SPEEDTRAIN:
        m_goodCount = 0;
        m_badCount = 0;
        m_ui->wordbox->hide();
        m_ui->letter->show();
        m_ui->clearTraining->show();
        m_ui->readButton->hide();
        m_ui->modeMenu->setText("Speed Training");
        m_ui->changeSequence->show();
        m_ui->changeWords->hide();
        m_ui->helpBar->setText("<font color=\"green\">Type the letter you hear ASAP.  The keying will get faster.</font>");
        m_ui->play->show();
        m_ui->WPM->show();
        m_playingMode = PLAYING;
        playButton(); // will change to "paused"
        startNextTrainingKey();
        break;
    case WORDS:
        m_goodCount = 0;
        m_badCount = 0;
        m_ui->wordbox->hide();
        m_ui->wordbox->clear();
        m_ui->letter->show();
        m_ui->clearTraining->hide();
        m_ui->readButton->hide();
        m_ui->modeMenu->setText("Word Training");
        m_ui->changeSequence->hide();
        m_ui->changeWords->show();
        m_ui->helpBar->setText("<font color=\"green\">Enter the word you hear and hit enter.</font>");
        m_ui->play->show();
        m_ui->WPM->show();
        m_playingMode = PLAYING;
        playButton(); // will change to "paused"
        m_maxWord = 2;
        startNextWord();
        break;
    case READ:
        m_ui->wordbox->show();
        m_ui->letter->hide();
        m_ui->clearTraining->hide();
        m_ui->readButton->show();
        m_ui->modeMenu->setText("Read to me!");
        m_ui->changeSequence->hide();
        m_ui->changeWords->hide();
        m_ui->helpBar->setText("<font color=\"green\">Enter text and hit the play button to hear the entire sequence.</font>");
        m_ui->play->hide();
        m_ui->WPM->hide();
        m_playingMode = PLAYING;
        playButton(); // will change to "paused"
        break;
    default:
        break;
    }
}

void Morse::switchSequence(int sequence) {
    m_trainingSequence = m_sequences.at(sequence);
    setSequence(m_trainingSequence, 1);
    clearStats();
    startNextTrainingKey();
}

void
Morse::add(QChar c, bool addpause)
{
    c = c.toLower();
    if (! code.contains(c))
        return;

    QList<ditdah>::iterator iter;
    QList<ditdah>::iterator endat = code[c]->end();

    bool lastWasPause = false;
    for(iter = code[c]->begin(); iter != endat; iter++)
    {
        lastWasPause = false;
        switch (*iter) {
        case DIT:
            add(m_dit);
            break;
        case DAH:
            add(m_dah);
            break;
        case PAUSE:
            add(m_pause);
            lastWasPause = true;
            break;
        case SPACE:
            add(m_space);
            lastWasPause = true;
            break;
        default:
            qWarning() << "error: illegal morse type added";
        }
        add(m_pause);
    }
    if (addpause && !lastWasPause) {
        add(m_letterPause);
    }
}

void Morse::add(const QString &textToAdd) {
    QString::const_iterator letter;
    QString::const_iterator lastLetter = textToAdd.end();

    clearList();
    for (letter = textToAdd.begin(); letter != lastLetter; ++letter) {
        add(*letter);
    }
}

void
Morse::createTones(int wpm)
{
    createTones(float(60.0/float(wpm*50.0)));
}

void
Morse::createTones(float ditSecs, int dahMult, int pauseMult, int letterPauseMult, int spaceMult)
{
    m_ditSecs = ditSecs;

    m_dit = new Generator(ditSecs);
    m_dit->start();

    m_dah = new Generator(ditSecs * dahMult);
    m_dah->start();

    m_pause = new Generator(ditSecs * pauseMult, 0);
    m_pause->start();

    m_letterPause = new Generator(ditSecs * letterPauseMult, 0);
    m_letterPause->start();

    m_space = new Generator(ditSecs * spaceMult, 0);
    m_space->start();

    m_playBuffer = new Generator(m_pause);
    m_playBuffer->start();
    connect(m_playBuffer, SIGNAL(generatorDone()), this, SLOT(generatorDone()));

    #include "morse_code.h"

    qDebug() << "created tones";
    connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(audioFinished(QAudio::State)));
}

void Morse::setStatus(const QString &status) {
    if (m_statusBar)
        m_statusBar->setText(status);
}

void Morse::setSequence(const QString &sequence, int currentlyAt) {
    if (m_sequenceLabel) {
        QString left = sequence.left(currentlyAt);
        QString right = sequence.right(sequence.length() - currentlyAt);
        m_sequenceLabel->setText("<font color=\"red\">" + left.toUpper() + "</font>" + right.toUpper());

        QChar theLetter = sequence[currentlyAt-1].toLower();
        QString newLetter = "<font color=\"red\">" + QString(theLetter.toUpper()) + "  ";
        QList<ditdah>::iterator it;
        QList<ditdah>::iterator end = code[theLetter]->end();
        for(it = code[theLetter]->begin(); it != end; ++it) {
            if (*it == DIT)
                newLetter = newLetter + " " + ".";
            else if (*it == DAH)
                newLetter = newLetter + " " + "-";
        }
        newLetter += "</font>";
        m_ui->letter->setText( newLetter );
    }
}

void Morse::setDoEntireSequence(bool value) {
    m_doEntireSequence = value;
}

Generator *
Morse::dit()
{
    return m_dit;
}

Generator *
Morse::dah()
{
    return m_dah;
}

Generator *
Morse::pause()
{
    return m_pause;
}

Generator *
Morse::letterPause()
{
    return m_letterPause;
}

Generator *
Morse::space()
{
    return m_space;
}
