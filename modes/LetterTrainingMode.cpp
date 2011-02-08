#include "LetterTrainingMode.h"
#include "Morse.h"

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
    m_ui->helpBar->setText("<font color=\"green\">Type the letter you hear ASAP.</font>");
    clear();

    setupSequences();
    setupWidgets(m_trainingSequence);
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
    return tr("In this mode characters will be sent and you need to press the appropriate key before a minimum time period, defined by the training speed preference.  "
              "As you get fast enough for particular characters then the next character in the sequence will be added.  Characters that you need the most work on will be "
              "sent more frequently than ones you're fast at responding to.");
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
                updateGraphs();
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
            updateGraphs();
            return m_lastTimes.last();
        }
    }
    qDebug() << "**** shouldn't get here: " << randTime << "," << totalTime;
    return QTime();
}
