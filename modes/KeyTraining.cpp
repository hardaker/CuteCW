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
}

void KeyTraining::handleKeyPress(QChar letterPressed)
{
    if (m_keyCount >= m_required)
        return;

    if (m_keyCount == 0) {
        m_startTime.start();
        m_keyTimes.push_back(0);
    } else {
        m_keyTimes.push_back(m_startTime.elapsed());
    }

    m_keyCount++;

    qDebug() << m_keyTimes;

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
    m_required = 4;
    m_keyCount = 0;
    m_keyTimes.clear();
    // pick new keying sequence, and display it
}

void KeyTraining::selectKeySequence()
{
    m_keySequence = completeCharacterSet[qrand() % completeCharacterSet.length()];
}

void KeyTraining::stop()
{
    // err...  nothing?
}



