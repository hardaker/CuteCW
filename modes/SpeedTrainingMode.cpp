#include <qdebug.h>

#include "SpeedTrainingMode.h"

SpeedTrainingMode::SpeedTrainingMode(Morse *parent, Ui::MainWindow *ui)
    : TrainingMode(parent, ui)
{
}

void SpeedTrainingMode::switchToMode() {
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
    clear();

    m_morse->setPlayingMode(Morse::PLAYING);
    playButton(); // will change to "paused"
}

void SpeedTrainingMode::startTimerToNextKey() {
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
        avetime = 1000*60.0/(50.0*float(m_morse->currentWPMAccept()));
        qDebug() << "setting avetime to: " << avetime;
    }
    delay  = (float(m_badCount + m_countWeight)/float(m_goodCount + m_countWeight)) * avetime;
    qDebug() << "delaying for: " << delay << " ms (good=" << m_goodCount << ", bad=" << m_badCount << ")";
    QTimer::singleShot(delay, this, SLOT(startNextTrainingKey()));
}

void SpeedTrainingMode::audioFinished(QAudio::State state) {
    //qDebug() << "audio state changed: " << state;
    if (state != QAudio::IdleState && state != QAudio::StoppedState)
        return;

    playButton();

    qDebug() << "speed train stop";
    if (m_morse->playingMode() != Morse::STOPPED) {
        startTimerToNextKey();
        m_lastTimes.push_back(QTime::currentTime());
    }
    m_morse->setPlayingMode(Morse::STOPPED);
}
