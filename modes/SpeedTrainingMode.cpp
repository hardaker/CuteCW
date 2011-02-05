#include <qdebug.h>

#include "SpeedTrainingMode.h"

SpeedTrainingMode::SpeedTrainingMode(Morse *parent, Ui::MainWindow *ui)
    : TrainingMode(parent, ui)
{
}

void SpeedTrainingMode::switchToMode() {
    m_ui->letter->show();
    m_ui->clearTraining->show();
    m_ui->modeMenu->setText("Speed Training");
    m_ui->changeSequence->show();
    //m_ui->forModes->show();
    m_ui->helpBar->setText("<font color=\"green\">Type the letter you hear ASAP.  The keying will get faster.</font>");
    m_ui->play->show();
    m_ui->WPM->show();
    clear();

    setupSequences();
    setupWidgets(m_trainingSequence);
}

void SpeedTrainingMode::play() {
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

    if (m_lastTimes.count() > 3) {
        // don't let them get *too* far behind
        m_badCount++;
        QTimer::singleShot(1000, this, SLOT(startNextSpeedKey()));  // Try again in a second
        return;
    }
    avetime = getStat(m_lastKey)->getAverageTime();
    qDebug() << "avetime: " << avetime;
    if (avetime == -1) {
        avetime = 1000.0;
        qDebug() << "setting avetime to: " << avetime;
    }
    delay  = plusMSecs + (float(m_badCount + m_countWeight)/float(m_goodCount + m_countWeight)) * avetime;
    qDebug() << "delaying for: " << delay << " ms (good=" << m_goodCount << ", bad=" << m_badCount << ")";
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
    return tr("Characters in the sequence will play at you with short pauses between each character.  As you guess more and more correctly the speed will increase "
              "and the spacing between the letters will decrease.");
}

QString SpeedTrainingMode::name()
{
    return tr("Word Training");
}
