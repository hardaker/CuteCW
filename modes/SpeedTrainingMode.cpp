#include <qdebug.h>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QFormLayout>

#include "SpeedTrainingMode.h"

SpeedTrainingMode::SpeedTrainingMode(Morse *parent, Ui::MainWindow *ui)
    : TrainingMode(parent, ui), m_countWeight(50)
{
}

void SpeedTrainingMode::switchToMode() {

    setupTrainingWidgets();

    m_ui->letter->show();
    m_ui->changeSequence->show();
    //m_ui->forModes->show();
    m_ui->helpBar->setText("<font color=\"green\">Type the letter you hear ASAP.  The keying will get faster.</font>");
    m_ui->play->show();
    m_ui->WPM->show();

    setupSequences();
    setupWidgets(m_trainingSequence, true, "S\np\ne\ne\nd\n \nR\na\nt\ni\nn\ng");
    updateGraphs();
}

void SpeedTrainingMode::play() {
    m_lastKeys.clear();
    m_lastTimes.clear();
    startNextSpeedKey();
}

void SpeedTrainingMode::startNextSpeedKey() {
    QTime endAt = startNextTrainingKey();
    QTime currentTime = QTime::currentTime();
    //qDebug() << "end at: " << endAt << ":" << endAt.msec() << ", now=" << currentTime << ":" << currentTime.msec();
    startTimerToNextKey(currentTime.msecsTo(endAt));
}

void SpeedTrainingMode::startTimerToNextKey(QTime plusTime) {
    startTimerToNextKey(plusTime.second()*1000 + plusTime.msec());
}

void SpeedTrainingMode::startTimerToNextKey(int plusMSecs) {
    if (plusMSecs < 0)
        plusMSecs = 0;

    float avetime, delay;
    if (runningMode() != RUNNING)
        return;

    if (plusMSecs <= 0) {
        qDebug() << "stop here!";
        plusMSecs = QTime::currentTime().msecsTo(m_morse->sequenceTime());
    }
    if (m_lastTimes.count() > 3) {
        // don't let them get *too* far behind
        m_badCount++;
        QTimer::singleShot(1000, this, SLOT(startTimerToNextKey()));  // Try again in a second
        return;
    }
    avetime = getStat(m_lastKey)->getAverageTime();
    qDebug() << "avetime: " << avetime;
    if (avetime == -1) {
        avetime = 1000.0;
        qDebug() << "setting avetime to: " << avetime;
    }
    delay  = plusMSecs + (float(m_badCount + m_countWeight)/float(m_goodCount + m_countWeight)) * avetime;
    qDebug() << "delaying for: t=" << avetime << " + ex=" << plusMSecs << " = " << delay << " ms (good=" << m_goodCount << ", bad=" << m_badCount
             << ", wt=" << m_countWeight << ")";
    QTimer::singleShot(delay, this, SLOT(startNextSpeedKey()));
}

void SpeedTrainingMode::handleKeyPress(QChar letterPressed) {
    if (runningMode() != RUNNING)
        return;

    // analyze they're keyed letter and immediately start playing a new one
    TrainingMode::handleKeyPress(letterPressed);
}

QString SpeedTrainingMode::helpText()
{
    return tr("Characters in the sequence will play at you with short pauses between each character.  The goal here is to train for both accuracy while "
              "under slight pressure as the next letter starts playing at you.  As you guess more and more correctly the speed will increase and incorrect guesses "
              "will slow down the keying rate.  "
              "As you get better, the spacing between the letters will decrease and will continue to challange you hopefully right near your learning point.");
}

QString SpeedTrainingMode::name()
{
    return tr("Speed Training");
}

QTime SpeedTrainingMode::startNextTrainingKey() {
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

        if (stat->getTryCount() == 0 && m_doEntireSequence) {
            thisTime = 1000*60.0/(50.0*float(m_morse->currentWPMAccept()));
        } else {
            thisTime = stat->getAverageTime();
        }

        totalTime += thisTime;
        if (minTime > thisTime)
            minTime = thisTime;

        if (stat->getTryCount() == 0 && !m_doEntireSequence) {

            // never keyed yet; do it immediately if we got this far
            qDebug() << "|keying: " << *letter;
            m_lastKey = *letter;
            m_lastKeys.append(*letter);
            setSequence(m_trainingSequence, letterCount);
            m_avewpmLabel->setText("All WPM: " + QString().setNum(msToPauseWPM(totalTime/letterCount)) + ", " +
                                  *letter + ": NEW");
            if (m_morse->trainingMode() == Morse::SPEEDTRAIN)
                setWPMLabel(msToPauseWPMF((float(m_badCount + m_countWeight)/float(m_goodCount + m_countWeight)) *
                                          totalTime/float(letterCount)));
            else
                setWPMLabel(msToPauseWPMF(totalTime/float(letterCount)));
            m_lastTimes.push_back(m_morse->playIt(*letter, false));
            updateGraphs();
            return m_lastTimes.last();
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

    // we have all the letters available to pick from at random

    // Calculate our magic constant:
    int numLetters = letters.count();
    int magicHelper;
    float currentAccept = m_morse->currentWPMAccept();

    if (numLetters > 2) {
        //magicHelper = int(((50.0 + float(numLetters) * float(maxPercent))*0.5 - 50.0)/((float(numLetters) + 1.0)*0.5 - 1.0));
        magicHelper = int(((currentAccept/2.0 + float(numLetters) * currentAccept)*0.5 - currentAccept/2.0)/((float(numLetters) + 1.0)*0.5 - 1.0));
    } else
        magicHelper = currentAccept*2;

    float totalWPM = 0;
    QList<QPair<QChar, float> >::iterator aletter;
    QList<QPair<QChar, float> >::iterator last = letters.end();

    for(aletter = letters.begin(); aletter != last; ++aletter) {
        float pauseWPM = qMax(msToPauseWPMF((*aletter).second), float(currentAccept));
        (*aletter).second = pauseWPM;
        totalWPM += pauseWPM;
    }

    m_avewpmLabel->setText("All WPM: " + QString().setNum(msToPauseWPM(totalTime/letterCount)) + ", " +
                          currentLetterGoal + " WPM: " + QString().setNum(msToPauseWPM(thisTime)));
    setWPMLabel(msToPauseWPMF((float(m_badCount + m_countWeight)/float(m_goodCount + m_countWeight)) * totalTime/float(letterCount)));

    // now pick a random time between 0 and the total of all the averages; averages with a slower speed are more likely

    float randWPM;

    randWPM = totalWPM*float(qrand())/float(RAND_MAX);

    float newTotal = 0;
    // qDebug() << "letter set random: " << randWPM << " total: " << totalTime << " min: " << minTime/2 << ", count: " << letters.count();
    QList<QPair<QChar, float> >::iterator search;
    setSequence(m_trainingSequence, letterCount);
    for(search = letters.begin(); search != last; ++search) {
        //qDebug() << "  -> " << (*search).first << "/" << (*search).second;
        newTotal += ((*search).second);
        if (newTotal > randWPM) {
            qDebug() << ">keying: " << (*search).first;
            m_lastKey = (*search).first;
            m_lastKeys.append((*search).first);
            m_lastTimes.push_back(m_morse->playIt((*search).first, false));
            updateGraphs();
            return m_lastTimes.last();
        }
    }
    qDebug() << "**** shouldn't get here: " << randWPM << "," << totalTime;
    return QTime();
}

bool SpeedTrainingMode::elapsedTimeWasTooLong(int msElapsed, MorseStat *stat) {
    if ((stat->getTryCount() > 0 && msElapsed > 5 * stat->getAverageTime()) ||
        (stat->getTryCount() == 0 && msElapsed > 5 * msToPauseWPM(m_morse->currentWPMAccept()))) {
        return true;
    }
    return false;
}

#define MULTFACTOR 10.0
void SpeedTrainingMode::updateGraphs()
{
#ifndef SMALL_DEVICE
    int fastestWPM = MULTFACTOR * m_morse->currentWPMGoal();
    int goalWPM = MULTFACTOR * m_morse->currentWPMAccept();
    foreach(QChar theLetter, m_trainingSequence) {
        float aveTime = getStat(theLetter)->getAverageTime();
        if (aveTime > 0) {
            fastestWPM = qMax(msToPauseWPM(aveTime / MULTFACTOR), fastestWPM);
        }
    }
    foreach(QChar theLetter, m_trainingSequence) {
        m_progressBars[theLetter]->setRange(0,fastestWPM);
        m_progressBars[theLetter]->setGoalBarLevel(goalWPM);
        float aveTime = getStat(theLetter)->getAverageTime();
        if (aveTime < 0)
            m_progressBars[theLetter]->setValue(0);
        else
            m_progressBars[theLetter]->setValue(msToPauseWPM(aveTime / MULTFACTOR));
    }
#endif
    // qDebug() << "max graph WPM: " << fastestWPM;
}

void SpeedTrainingMode::loadSettings(QSettings &settings)
{
    m_countWeight = settings.value("speedtraining/countWeight", 50).toInt();
    loadStats(settings);
}

void SpeedTrainingMode::saveSettings(QSettings &settings)
{
    settings.setValue("speedtraining/countWeight", m_countWeight);
    saveStats(settings);
}

QBoxLayout *SpeedTrainingMode::getPrefsLayout()
{
    QHBoxLayout *hbox = new QHBoxLayout();
    QFormLayout *form = new QFormLayout();
    hbox->addLayout(form);

    form->addRow(tr("Speed Acceptance Rate"), m_acceptRateBox = new QSpinBox());
    m_acceptRateBox->setRange(1,100);
    m_acceptRateBox->setValue(m_morse->currentWPMAccept());

    form->addRow(tr("Speed Increasing Aggressiveness"), m_countWeightBox = new QSpinBox());
    m_countWeightBox->setRange(1,1000);
    m_countWeightBox->setValue(m_countWeight);

    QLabel *helpLabel = new QLabel(tr("(lower = speed increases more quickly)"));
    QFont font = helpLabel->font();
    font.setItalic(true);
    helpLabel->setFont(font);
    form->addRow("", helpLabel);

    return hbox;
}

void SpeedTrainingMode::acceptPrefs()
{
    m_morse->setWPMAccept(m_acceptRateBox->value());
    m_countWeight = m_countWeightBox->value();
}
