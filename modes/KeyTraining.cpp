#include "KeyTraining.h"
#include "Morse.h"
#include "MainWindow.h"

#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QTextEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QHBoxLayout>
#include <qdebug.h>

KeyTraining::KeyTraining(Morse *morse, Ui::MainWindow *ui)
    : MorseMode(morse, ui), m_required(0), m_keyCount(0)
{

}

QString KeyTraining::name()
{
    return tr("KeyTraining");
}

QString KeyTraining::helpText()
{
    return tr("this is for practing keying.");
}

void KeyTraining::switchToMode()
{
    m_ui->letter->show();
    m_ui->helpBar->setText("<font color=\"green\">Key the letters using any keyborad key as a straight key.</font>");
    m_ui->play->show();

    m_ui->forModes->addWidget(m_timingDisplay = new KeyTimingDisplay());
}

void KeyTraining::handleKeyPress(QChar letterPressed)
{
    if (m_keyCount >= m_required)
        return;

    if (m_keyCount == 0) {
        m_startTime.start();
        m_keyedTimes.push_back(0);
    } else {
        m_keyedTimes.push_back(m_startTime.elapsed());
    }

    m_keyCount++;

    qDebug() << m_keyedTimes;

    // XXX: recond the timing
}

void KeyTraining::handleKeyRelease(QChar letterPressed)
{
    handleKeyPress(letterPressed);  // doesn't really matter whether it's up/down
}

void KeyTraining::play()
{
    selectKeySequence();
    m_ui->letter->setText(m_keySequence);
    m_keyCount = 0;
    m_keyedTimes.clear();
    // pick new keying sequence, and display it
}

void KeyTraining::selectKeySequence()
{
    int currentMS = 0;
    m_requiredTimes.clear();

    // select the sequence we want them to key
    m_keySequence = completeCharacterSet[qrand() % completeCharacterSet.length()];

    // translate the sequence into a list of timings (by MS)
    foreach(QChar letter, m_keySequence) {
        const QList<Morse::ditdah> *code = m_morse->getLetterCode(letter);
        // XXX: treat spaces specially (pop the last pause, add a longer one)
        foreach(Morse::ditdah segment, *code) {
            m_requiredTimes.push_back(currentMS);  // save the current spot as the beginning
            currentMS += ditDahToMS(segment);      // mark the end as the next spot
            m_requiredTimes.push_back(currentMS);  // save this as the end spot
            currentMS += m_morse->pauseSecsMS();   // add in the pause till the next letter
        }
    }

    m_required = m_requiredTimes.count();
}

int KeyTraining::ditDahToMS(Morse::ditdah it) {
    switch(it) {
    case Morse::DIT:
        return m_morse->ditSecsMS();
    case Morse::DAH:
        return m_morse->dahSecsMS();
    default:
        // uh oh
        exit(99);
    }
}

void KeyTraining::stop()
{
    // err...  nothing?
}



