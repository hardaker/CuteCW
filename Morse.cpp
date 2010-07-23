#include "Morse.h"

#include <QtCore/QSettings>
#include <QtGui/QMenu>
#include <qdebug.h>

#include "MainWindow.h"

#define WPMGOAL   20
#define WPMACCEPT 15

Morse::Morse()
    : QObject(), m_parent(0), m_audioOutput(), m_dit(0), m_dah(0), m_space(0), m_pause(0), m_letterPause(0), m_playingMode(STOPPED), m_gameMode(PLAY),
    m_currentWPMGoal(WPMGOAL), m_currentWPMAccept(WPMACCEPT), m_statusBar(0), m_sequenceLabel(0), m_ui(0)
{
    setupSequences();
}

Morse::Morse(MainWindow *parent, QAudioOutput *output, Ui::MainWindow *ui)
    : QObject(parent), m_parent(parent), m_audioOutput(output), m_dit(0), m_dah(0), m_space(0), m_pause(0), m_letterPause(0), m_playingMode(STOPPED), m_gameMode(PLAY),
    m_currentWPMGoal(WPMGOAL), m_currentWPMAccept(WPMACCEPT),
    m_statusBar(ui->status), m_sequenceLabel(ui->sequence), m_ui(ui)
{


    createTones(WPMGOAL);
    setStatus("ready: Play Mode");
    qsrand(QTime::currentTime().msec());
    loadSettings();
    switchMode(Morse::PLAY);

    connect(m_ui->readButton, SIGNAL(clicked()), this, SLOT(readIt()));
    connect(m_ui->clearTraining, SIGNAL(clicked()), this, SLOT(clearStatsButton()));

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(startNextTrainingKey()));
    setupSequences();
}

void Morse::setupSequences() {
    m_trainingSequence = "kmuresnaptlwi.jz=foy,vg5/q92h38b?47c1d60x";
    m_sequences.append("kmuresnaptlwi.jz=foy,vg5/q92h38b?47c1d60x");
    m_sequences.append("kmuresnaptl");
    m_sequences.append("wi.jz=foy,");
    m_sequences.append("vg5/q92h38");
    m_sequences.append("b?47c1d60x");
    m_sequences.append("abcdefghijklmnopqrstuvwxyz1234567890.,/=?");

    m_signalMapper = new QSignalMapper();
    QMenu *modeMenu = new QMenu(m_ui->changeSequence);
    m_ui->changeSequence->setMenu(modeMenu);

    QAction *action = modeMenu->addAction("Full Koch");
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::KOCH);

    action = modeMenu->addAction("Koch part 1");
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::KOCH1);

    action = modeMenu->addAction("Koch part 2");
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::KOCH2);

    action = modeMenu->addAction("Koch part 3");
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::KOCH3);

    action = modeMenu->addAction("Koch part 4");
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::KOCH4);

    action = modeMenu->addAction("Alphabet");
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::ALPHABET);

    connect(m_signalMapper, SIGNAL(mapped(int)), this, SLOT(switchSequence(int)));
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
}

void Morse::clearStatsButton() {
    clearStats();
    startNextTrainingKey();
}

void
Morse::playSequence()
{
    m_playBuffer->restartData();
    m_playBuffer->start();
    m_playingMode = PLAYING;
    m_audioOutput->start(m_playBuffer);
    return;
}

void Morse::readIt() {
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
    if (m_playingMode == STOPPED) {
        playSequence();
    }
}

void Morse::addAndPlayIt(QChar c) {
    if (m_playingMode == STOPPED) {
        clearList();
        add(letterPause());
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

void Morse::handleKeyResponse(QChar letterPressed) {
    int msElapsed = m_lastTime.elapsed() - m_ditSecs; // subtract off blank-after time
    qDebug() << "Training response: elapsed " << msElapsed << "ms (" << msToPauseWPM(msElapsed) << " WPM)";
    MorseStat *pressedStat = getStat(letterPressed);
    if (pressedStat->getTryCount() > 0 && msElapsed > 5 * pressedStat->getAverageTime()) {
        qDebug() << "ignoring key press; too long and probably an interruption";
        return;
    }
    m_ui->lastwpm->setText(QString().setNum(msToPauseWPM(msElapsed)));
    // if the keyed incorrectly, penalize them 3 times their average
    if (letterPressed == m_lastKey) {
        pressedStat->addTime(msElapsed);
    } else {
        pressedStat->addTime(3.0 * pressedStat->getAverageTime());
        getStat(m_lastKey)->addTime(3.0 * getStat(m_lastKey)->getAverageTime());
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
    default:
        qDebug() << "ignoring key: " << newletter;
    }
}

int Morse::msToWPM(float ms) {
    return (60*1000)/ms; // XXX: fix me; doesn't include keying times
}

int Morse::msToPauseWPM(float ms) {
    // 3 dits in length is the pause between letter spacing
    float pauseLength = 3.0 * m_ditSecs;
    // calculate the WPM based on the space it took for the letter to be identified during the pause
    qDebug() << "pause length: " << pauseLength << ", recorded time: " << ms << ", % = " << (pauseLength * 1000.0 * 100.0 / ms );
    return int(float(m_currentWPMGoal) * pauseLength * 1000.0 / ms);
}

void Morse::startNextTrainingKey() {
    int letterCount = 0;
    QList<QPair<QChar, float> > letters;
    float totalTime = 0.0, thisTime, minTime = 0.0;

    QString::iterator letter;
    QString::iterator lastLetter = m_trainingSequence.end();
    for(letter = m_trainingSequence.begin(); letter != lastLetter; ++letter) {
        letterCount++;
        MorseStat *stat = getStat(*letter);
        thisTime = stat->getAverageTime();
        totalTime += thisTime;
        if (minTime > thisTime)
            minTime = thisTime;
        if (thisTime < 0) {
            // never keyed yet; do it immediately if we got this far
            setStatus("Starting a new letter: " + QString(*letter));
            addAndPlayIt(*letter);
            qDebug() << "------- keying: " << *letter;
            m_lastTime = QTime::currentTime(); // XXX: only added to test on broken linux audio
            qDebug() << "setting last time to " << m_lastTime;
            m_lastKey = *letter;
            setSequence(m_trainingSequence, letterCount);
            return;
        }

        qDebug() << "adding " << *letter << " / " << thisTime << " / " << msToPauseWPM(thisTime);
        letters.append(QPair<QChar, float>(*letter, thisTime));

        if(msToPauseWPM(thisTime) <= m_currentWPMAccept) {
            // we're not fast enough; break here
            qDebug() << " too slow: " << *letter << " / " << thisTime << " / " << msToPauseWPM(thisTime);
            break;
        }
    }

    m_ui->avewpm->setText(QString().setNum(msToPauseWPM(totalTime/letterCount)));
    // now pick a random time between 0 and the total of all the averages; averages with a slower speed are more likely
    // XXX: probably could use a weighted average (subtract off min speed from all speeds)?
    
    float randTime, subTime = 0.0;
    if (m_badLetterWeighting == HIGH) {
        randTime = (totalTime - minTime * letters.count())*float(qrand())/float(RAND_MAX);
        subTime = minTime;
    } else
        randTime = totalTime*float(qrand())/float(RAND_MAX);
    float newTotal = 0;
    qDebug() << "letter set random: " << randTime << " total: " << totalTime << " min: " << minTime << ", count: " << letters.count();
    QList<QPair<QChar, float> >::iterator search;
    QList<QPair<QChar, float> >::iterator last = letters.end();
    setSequence(m_trainingSequence, letterCount);
    for(search = letters.begin(); search != last; ++search) {
        qDebug() << "  -> " << (*search).first << "/" << (*search).second;
        newTotal += ((*search).second - subTime);
        if (newTotal > randTime) {
            qDebug() << "------- keying: " << (*search).first;
            addAndPlayIt((*search).first);
            m_lastTime = QTime::currentTime(); // XXX: only added to test on broken linux audio
            m_lastKey = (*search).first;
            m_lastKeys.append((*search).first);
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

void
Morse::audioFinished(QAudio::State state)
{
    if (state != QAudio::IdleState && state != QAudio::StoppedState)
        return;
    switch (m_gameMode) {
    case READ:
        // add in next letter and display it
        readNextLetter();
        break;

    case SPEEDTRAIN:
        m_timer.stop();
        m_timer.start(getStat(m_lastKey)->getAverageTime());
        break;

    default:
        m_lastTime = QTime::currentTime();
        m_playingMode = STOPPED;
        qDebug() << "time stopped at" << m_lastTime;
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
    switch (m_gameMode) {
    case PLAY:
        m_ui->wordbox->hide();
        m_ui->letter->hide();
        m_ui->clearTraining->hide();
        m_ui->readButton->hide();
        m_ui->changeSequence->hide();
        m_ui->modeMenu->setText("Play Morse Code");
        break;
    case TRAIN:
        m_ui->wordbox->hide();
        m_ui->letter->show();
        m_ui->clearTraining->show();
        m_ui->readButton->hide();
        m_ui->modeMenu->setText("Recognition Training");
        m_ui->changeSequence->show();
        startNextTrainingKey();
        break;
    case READ:
        m_ui->wordbox->show();
        m_ui->letter->hide();
        m_ui->clearTraining->hide();
        m_ui->readButton->show();
        m_ui->modeMenu->setText("Read to me!");
        m_ui->changeSequence->hide();
        break;
    case SPEEDTRAIN:
        m_ui->wordbox->hide();
        m_ui->letter->show();
        m_ui->clearTraining->show();
        m_ui->readButton->hide();
        m_ui->modeMenu->setText("Speed Training");
        m_ui->changeSequence->show();
        startNextTrainingKey();
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

    bool lastWasPause;
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

voidd
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
        m_ui->letter->setText("<font color=\"red\">" + QString(sequence[currentlyAt-1].toUpper()) + "</font>");
    }
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
