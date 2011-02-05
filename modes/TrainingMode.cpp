#include "TrainingMode.h"
#include "Morse.h"


#include <qdebug.h>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>

TrainingMode::TrainingMode(Morse *parent, Ui::MainWindow *ui)
    : MorseMode(parent, ui), m_doEntireSequence(false), m_maxBadLetters(2)
{
    setupSequences();
}

void TrainingMode::setupSequences() {
    m_trainingSequence = "kmuresnaptlwi.jz=foy,vg5/q92h38b?47c1d60x";
    m_sequences.append("kmuresnaptlwi.jz=foy,vg5/q92h38b?47c1d60x");
    m_sequences.append("kmuresnaptl");
    m_sequences.append("wi.jz=foy,");
    m_sequences.append("vg5/q92h38");
    m_sequences.append("b?47c1d60x");
    m_sequences.append("abcdefghijklmnopqrstuvwxyz");
    m_sequences.append("abcdef");
    m_sequences.append("ghijklm");
    m_sequences.append("nopqrst");
    m_sequences.append("tuvwxyz");
    m_sequences.append("1234567890");
    m_sequences.append(".,/=?");
    m_sequences.append("abcdefghijklmnopqrstuvwxyz1234567890.,/=?");

    // Koch sequences
    m_sequenceSignalMapper = new QSignalMapper();
    QMenu *modeMenu = new QMenu(m_ui->changeSequence);
    m_ui->changeSequence->setMenu(modeMenu);

    QMenu *subMenu = modeMenu->addMenu(tr("Koch"));

    QAction *action = subMenu->addAction("Full Koch");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) KOCH);

    action = subMenu->addAction("Koch part 1");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) KOCH1);

    action = subMenu->addAction("Koch part 2");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) KOCH2);

    action = subMenu->addAction("Koch part 3");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) KOCH3);

    action = subMenu->addAction("Koch part 4");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) KOCH4);


    // Alphabet sequences
    subMenu = modeMenu->addMenu(tr("Alphabet"));

    action = subMenu->addAction("Full");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) ALPHABET);

    action = subMenu->addAction("A-F");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) AF);

    action = subMenu->addAction("G-M");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) GM);

    action = subMenu->addAction("N-T");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) NT);

    action = subMenu->addAction("U-Z");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) UZ);

    // numbers and symbols

    action = modeMenu->addAction("Numbers");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) NUMBERS);

    action = modeMenu->addAction("Symbols");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) SYMBOLS);

    action = modeMenu->addAction("Symbols");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) EVERYTHING);

    action = modeMenu->addAction("Custom");
    connect(action, SIGNAL(triggered()), this, SLOT(chooseCustomeSequence()));

    // Connect the mapper
    connect(m_sequenceSignalMapper, SIGNAL(mapped(int)), this, SLOT(switchSequence(int)));
}

void TrainingMode::chooseCustomeSequence() {
    CustomSequenceDialog dialog(m_sequences[EVERYTHING], m_trainingSequence);
    if (dialog.exec() == QDialog::Accepted) {
        QString results = dialog.getResults();
        switchSequence(results);
    } else
        qDebug() << "maybe another time";
}

MorseStat *TrainingMode::getStat(const QChar &key) {
    if (! m_stats.contains(key))
        m_stats[key] = new MorseStat(0);
    return m_stats[key];
}

void TrainingMode::clear()  {
    QMap<QChar, MorseStat *>::iterator item;
    QMap<QChar, MorseStat *>::iterator end = m_stats.end();
    for(item = m_stats.begin(); item != end; ++item) {
        delete *item;
    }
    m_stats.clear();
    m_lastKeys.clear();
    m_lastTimes.clear();
    MorseMode::clear();
    m_goodCount = 0;
    m_badCount = 0;

    m_ui->avewpm->setText("All WPM: [None], K WPM: NEW");

    if (runningMode() == RUNNING)
        playButton(); // pretend a pause was pressed too
}

void TrainingMode::play() {
    startNextTrainingKey();
}

void TrainingMode::audioStopped() {
    qDebug() << "audio stopped";
    // now done based on predicted time
    // m_lastTimes.push_back(QTime::currentTime());
}

void TrainingMode::handleKeyPress(QChar letterPressed) {
    if (runningMode() != RUNNING)
        return;

    QTime now = QTime::currentTime();

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

    int msElapsed = lastTime.msecsTo(now) - m_morse->ditSecs(); // subtract off blank-after time
    if (msElapsed <= 0)
        msElapsed = 1;
    qDebug() << "Training response: elapsed " << msElapsed << "ms (" << msToPauseWPM(msElapsed) << " WPM)";
    MorseStat *pressedStat = getStat(letterPressed);

    // if the user took a *really* long time, ignore the key press and assume they got distracted from training
    if ((pressedStat->getTryCount() > 0 && msElapsed > 5 * pressedStat->getAverageTime()) ||
        (pressedStat->getTryCount() == 0 && msElapsed > 5 * msToPauseWPM(m_morse->currentWPMAccept()))) {
        qDebug() << "ignoring key press; too long and probably an interruption";
        return;
    }

    // set the last WPM record on the display
    QString WPM = QString().setNum(msToPauseWPM(msElapsed)) + " ";
    foreach(QChar thechar, m_lastKeys) {
        WPM = WPM + "-";
    }
    qDebug() << "WPM text: " << WPM;
    m_ui->lastwpm->setText(WPM);

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

QTime TrainingMode::startNextTrainingKey() {
    if (runningMode() != RUNNING)
        return QTime();
    qDebug() << "--- Start next training key";
    int letterCount = 0;
    QList<QPair<QChar, float> > letters;
    float totalTime = 0.0, thisTime = 0.0, minTime = 0.0;
    MorseStat *stat = 0;
    QString currentLetterGoal;
    int badLetters = 0;

    //if (m_morse->audioMode() == Morse::PLAYING)
    //    return;

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
                thisTime = 1000*60.0/(50.0*float(m_morse->currentWPMAccept()));
            } else {
                // never keyed yet; do it immediately if we got this far
                qDebug() << "|keying: " << *letter;
                m_lastKey = *letter;
                m_lastKeys.append(*letter);
                setSequence(m_trainingSequence, letterCount);
                m_ui->avewpm->setText("All WPM: " + QString().setNum(msToPauseWPM(totalTime/letterCount)) + ", " +
                                      *letter + ": NEW");
                if (m_morse->trainingMode() == Morse::SPEEDTRAIN)
                    setWPMLabel(msToPauseWPMF((float(m_badCount + m_countWeight)/float(m_goodCount + m_countWeight)) *
                                              totalTime/float(letterCount)));
                else
                    setWPMLabel(msToPauseWPMF(totalTime/float(letterCount)));
                m_lastTimes.push_back(m_morse->playIt(*letter));
                return m_lastTimes.last();
            }
        }

        //qDebug() << "  adding " << *letter << " / " << thisTime << " / " << msToPauseWPM(thisTime);
        letters.append(QPair<QChar, float>(*letter, thisTime));

        if(msToPauseWPM(thisTime) <= m_morse->currentWPMAccept()) {
            qDebug() << " too slow: " << *letter << " / " << thisTime << " / " << msToPauseWPM(thisTime);
            if (++badLetters >= m_maxBadLetters) {
                // we're not fast enough; break here
                break;
            }
        }
    }

    // They have the whole sequence active at this point

    m_ui->avewpm->setText("All WPM: " + QString().setNum(msToPauseWPM(totalTime/letterCount)) + ", " +
                          currentLetterGoal + " WPM: " + QString().setNum(msToPauseWPM(thisTime)));
    if (m_morse->trainingMode() == Morse::SPEEDTRAIN)
        setWPMLabel(msToPauseWPMF((float(m_badCount + m_countWeight)/float(m_goodCount + m_countWeight)) * totalTime/float(letterCount)));
    else
        setWPMLabel(msToPauseWPMF(totalTime/float(letterCount)));
    // now pick a random time between 0 and the total of all the averages; averages with a slower speed are more likely
    // XXX: probably could use a weighted average (subtract off min speed from all speeds)?

    float randTime, subTime = 0.0;
    if (m_morse->badLetterWeighting() == Morse::HIGH) {
        subTime = minTime/2;
        randTime = (totalTime - subTime * letters.count())*float(qrand())/float(RAND_MAX);
    } else
        randTime = totalTime*float(qrand())/float(RAND_MAX);
    float newTotal = 0;
    // qDebug() << "letter set random: " << randTime << " total: " << totalTime << " min: " << minTime/2 << ", count: " << letters.count();
    QList<QPair<QChar, float> >::iterator search;
    QList<QPair<QChar, float> >::iterator last = letters.end();
    setSequence(m_trainingSequence, letterCount);
    for(search = letters.begin(); search != last; ++search) {
        //qDebug() << "  -> " << (*search).first << "/" << (*search).second;
        newTotal += ((*search).second - subTime);
        if (newTotal > randTime) {
            qDebug() << ">keying: " << (*search).first;
            m_lastKey = (*search).first;
            m_lastKeys.append((*search).first);
            m_lastTimes.push_back(m_morse->playIt((*search).first));
            return m_lastTimes.last();
        }
    }
    qDebug() << "**** shouldn't get here: " << randTime << "," << totalTime;
    return QTime();
}

void TrainingMode::setWPMLabel(float wpm) {
    // set the last WPM record on the display
    QString WPM = "Speed Rating: " + QString().setNum(wpm, 'g', 2) + " ";
    foreach(QChar thechar, m_lastKeys) {
        WPM = WPM + "-";
    }
    m_ui->WPM->setText(WPM);
}

void TrainingMode::switchSequence(int sequence) {
    switchSequence(m_sequences.at(sequence));
}

void TrainingMode::switchSequence(const QString &sequence) {
    m_trainingSequence = sequence;
    setSequence(m_trainingSequence, 1);
    clear();
    setupKeyWidgets(m_trainingSequence);
    startNextTrainingKey();
}

void TrainingMode::setSequence(const QString &sequence, int currentlyAt) {
    if (m_morse->m_sequenceLabel) {
        QString left = sequence.left(currentlyAt);
        QString right = sequence.right(sequence.length() - currentlyAt);
        m_morse->m_sequenceLabel->setText("<font color=\"red\">" + left.toUpper() + "</font>" + right.toUpper());

        QChar theLetter = sequence[currentlyAt-1].toLower();
        QString newLetter = "<font color=\"red\">" + QString(theLetter.toUpper()) + "&nbsp;&nbsp;&nbsp;&nbsp;";
        QList<Morse::ditdah>::iterator it;
        QList<Morse::ditdah>::iterator end = m_morse->code[theLetter]->end();
        for(it = m_morse->code[theLetter]->begin(); it != end; ++it) {
            if (*it == Morse::DIT)
                newLetter = newLetter + " " + ".";
            else if (*it == Morse::DAH)
                newLetter = newLetter + " " + "-";
        }
        newLetter += "</font>";
        m_ui->letter->setText( newLetter );
    }
}

void TrainingMode::setDoEntireSequence(bool value) {
    m_doEntireSequence = value;
    m_doEntireSequenceButton->setChecked(value);
    qDebug() << "Switching to " << value;
}

void TrainingMode::setupWidgets(const QString &sequence)
{
    setupKeyWidgets(sequence);

    // Create the preference items in the quick menu
    m_doEntireSequenceButton = m_optionsMenu->addAction(tr("Use &Entire Sequence"));
    m_doEntireSequenceButton->setCheckable(true);
    m_doEntireSequenceButton->setChecked(false);
    setSequence(m_trainingSequence, 1);
    connect(m_doEntireSequenceButton, SIGNAL(toggled(bool)), this, SLOT(setDoEntireSequence(bool)));
}
