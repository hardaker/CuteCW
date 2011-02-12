#include "LetterTrainingMode.h"
#include "Morse.h"

#include <stdlib.h>

#ifdef Q_OS_WIN32
// cross compiling pulls in the wrong stdlib.h for some reason
#undef RAND_MAX
#define RAND_MAX 0x7FFF
#endif

#include <qdebug.h>

LetterTrainingMode::LetterTrainingMode(Morse *parent, Ui::MainWindow *ui)
    : TrainingMode(parent, ui)
{
}

void LetterTrainingMode::switchToMode() {
    m_ui->letter->show();
    m_ui->play->show();
    setupTrainingWidgets();
    m_ui->changeSequence->show();
    m_ui->helpBar->setText("<font color=\"green\">Type the letter you hear as accurately as possible within a half a second.</font>");

    setupSequences();
    setupWidgets(m_trainingSequence);
    updateGraphs();
}

void LetterTrainingMode::handleKeyPress(QChar letterPressed) {
    if (runningMode() != RUNNING)
        return;

    // analyze they're keyed letter and immediately start playing a new one
    TrainingMode::handleKeyPress(letterPressed);
    startNextTrainingKey();
}

QString LetterTrainingMode::helpText()
{
    return tr("<p>In this mode characters will be played for you. You will need to press the appropriate within a half-a-second.  Accuracy is more important than speed."
              "In order to continue advancing in the character set you will need to achieve at least a 90% accuracy rate on the letters, with at least 5 guesses for each"
              "letter under your belt."
              "Characters that you need the most help with will be sent more frequently than the characters you're good at.</p>"
              "<p>The important lesson is to learn to recognize and automatically type the letter you hear, without stressing out about the next letter. Be <b>sure</b> "
              "to aim for accuracy not speed in this mode.</p>"
              "<p>After you succeed in learning the characters, continue on to the speed training mode next where you'll be judged on both accuracy and speed.");
}

QString LetterTrainingMode::name()
{
    return tr("Recognition Training");
}

QTime LetterTrainingMode::startNextTrainingKey() {
    if (runningMode() != RUNNING)
        return QTime();
    qDebug() << "--- Start next training key";
    int letterCount = 0;
    QList<QPair<QChar, int> > letters;
    int totalPercent = 0, thisPercent = 0, minPercent = 100, maxPercent = 0;
    MorseStat *stat = 0;
    QString currentLetterGoal;
    int badLetters = 0;

    m_lastKeys.clear();
    m_lastTimes.clear();

    //if (m_morse->audioMode() == Morse::PLAYING)
    //    return;

    QString::iterator letter;
    QString::iterator lastLetter = m_trainingSequence.end();
    for(letter = m_trainingSequence.begin(); letter != lastLetter; ++letter) {
        letterCount++;
        currentLetterGoal = (*letter).toUpper();
        stat = getStat(*letter);

        // if doEntireSequence is turned on, then estimate they're getting an F at least, but not a zero.
        if (stat->getTryCount() == 0 && m_doEntireSequence)
            thisPercent = 50;
        else
            thisPercent = stat->getGoodPercentage(m_minimumTries);

        totalPercent += thisPercent;
        minPercent = qMin(minPercent, thisPercent);
        maxPercent = qMax(minPercent, thisPercent);

        if (stat->getTryCount() == 0 && !m_doEntireSequence) {
            // never keyed yet; do it immediately if we got this far
            qDebug() << "|keying: " << *letter;
            m_lastKey = *letter;
            m_lastKeys.append(*letter);
            setSequence(m_trainingSequence, letterCount);
            m_ui->avewpm->setText("All Percentage: " + QString().setNum(totalPercent/letterCount) + ", " +
                                  *letter + ": NEW");
            setWPMLabel(thisPercent);
            m_lastTimes.push_back(m_morse->playIt(*letter));
            updateGraphs();
            return m_lastTimes.last();
        }

        qDebug() << "  adding " << *letter << " / " << thisPercent;
        letters.append(QPair<QChar, int>(*letter, thisPercent));

        if(thisPercent <= m_percentGoal || stat->getTryCount() < m_minimumTries) {
            qDebug() << "   too low: " << *letter << " / " << thisPercent << " / " << stat->getTryCount();
            if (++badLetters >= m_maxBadLetters || stat->getTryCount() <= m_minimumTries) {
                // enough letters aren't accurate; break here
                break;
            }
        }
    }

    // we have all the letters available to pick from at random

    // Calculate our magic constant:
    int numLetters = letters.count();
    int magicHelper;
    typedef QPair<QChar, int> ourpair;

    if (numLetters > 2) {
        //magicHelper = int(((50.0 + float(numLetters) * float(maxPercent))*0.5 - 50.0)/((float(numLetters) + 1.0)*0.5 - 1.0));
        magicHelper = int(((50.0 + float(numLetters) * 100.0)*0.5 - 50.0)/((float(numLetters) + 1.0)*0.5 - 1.0));
    } else
        magicHelper = 200;

    totalPercent = 0;
    QList<QPair<QChar, int> >::iterator aletter;
    QList<QPair<QChar, int> >::iterator last = letters.end();
    for(aletter = letters.begin(); aletter != last; ++aletter) {
        (*aletter).second = magicHelper - (*aletter).second;
        totalPercent += (*aletter).second;
    }

    m_ui->avewpm->setText("All Percentage: " + QString().setNum(totalPercent/letterCount) + ", " +
                          *letter + ": NEW");
    setSequence(m_trainingSequence, letterCount);

    setWPMLabel(totalPercent/letterCount);
    // now pick a random time between 0 and the total of all the averages; averages with a slower speed are more likely

    float randPercent;
    float newTotal = 0;

    randPercent = float(totalPercent)*float(qrand())/float(RAND_MAX);

    qDebug() << "randomizing: " << randPercent << " total: " << totalPercent << " min/max: " << minPercent
             << "/" << maxPercent << ", count: " << letters.count() << ", magic: " << magicHelper;
    QList<QPair<QChar, int> >::iterator search;
    for(search = letters.begin(); search != last; ++search) {
        qDebug() << "  -> " << (*search).first << "/" << (*search).second;
        newTotal += ((*search).second);
        if (newTotal > randPercent) {
            qDebug() << ">keying: " << (*search).first;
            m_lastKey = (*search).first;
            m_lastKeys.append((*search).first);
            m_lastTimes.push_back(m_morse->playIt((*search).first));
            updateGraphs();
            return m_lastTimes.last();
        }
    }
    qDebug() << "**** shouldn't get here: " << randPercent << "," << totalPercent;
    return QTime();
}

bool LetterTrainingMode::elapsedTimeWasTooLong(int msElapsed, MorseStat *stat) {
    Q_UNUSED(stat);
    if (msElapsed > 1000) {
        return true;
    }
    return false;
}

void LetterTrainingMode::updateGraphs()
{
#ifndef SMALL_DEVICE
    foreach(QChar theLetter, m_trainingSequence) {
        m_progressBars[theLetter]->setRange(0, 100);
        m_progressBars[theLetter]->setValue(getStat(theLetter)->getGoodPercentage(m_minimumTries));
    }
    // qDebug() << "max graph WPM: " << fastestWPM;
#endif
}

void LetterTrainingMode::loadSettings(QSettings &settings)
{
    loadStats(settings);
}

void LetterTrainingMode::saveSettings(QSettings &settings)
{
    saveStats(settings);
}
