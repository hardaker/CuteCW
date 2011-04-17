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
#include <QtGui/QVBoxLayout>
#include <QtGui/QGridLayout>
#include <qdebug.h>

KeyTraining::KeyTraining(Morse *morse, Ui::MainWindow *ui)
    : MorseMode(morse, ui), m_letterCount(3), m_required(0), m_keyCount(0)
{

}

QString KeyTraining::name()
{
    return tr("Straight Key Training");
}

QString KeyTraining::helpText()
{
    return tr("this is for practing keying.");
}

QString KeyTraining::icon()
{
    return QString(":/icons/64x64/keying.png");
}

void KeyTraining::changeNumberOfLetters(int value) {
    m_letterCount = value;
    selectKeySequence();
}

void KeyTraining::switchToMode()
{
    m_ui->letter->show();
    m_ui->helpBar->setText("<font color=\"green\">Key the letters using any keyborad key as a straight key.</font>");
    m_ui->play->show();

    QVBoxLayout *vbox = new QVBoxLayout();
    m_ui->forModes->addLayout(vbox);

    QHBoxLayout *hbox = new QHBoxLayout(), *leftBox = new QHBoxLayout();
    hbox->addLayout(leftBox);
    vbox->addLayout(hbox);

    leftBox->addWidget(new QLabel(tr("Number of Letters:")));
    QSpinBox *lettersBox = new QSpinBox();
    lettersBox->setValue(m_letterCount);
    lettersBox->setRange(1,10);
    connect(lettersBox, SIGNAL(valueChanged(int)), this, SLOT(changeNumberOfLetters(int)));
    leftBox->addWidget(lettersBox);

    setupWPMWidgets(hbox, WPMRATE);

    vbox->addWidget(m_timingDisplay = new KeyTimingDisplay());
    vbox->setStretchFactor(m_timingDisplay, 2);

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(new QLabel("Dit Accuracy:"), 0, 0);
    grid->addWidget(new QLabel("Dah Accuracy:"), 0, 2);
    grid->addWidget(new QLabel("Pause Accuracy:"), 1, 0);
    grid->addWidget(new QLabel("Letter Spacing:"), 1, 2);

    grid->addWidget(m_ditStats = new QLabel(""), 0, 1);
    grid->addWidget(m_dahStats = new QLabel(""), 0, 3);
    grid->addWidget(m_pauseStats = new QLabel(""), 1, 1);
    grid->addWidget(m_letterPauseStats = new QLabel(""), 1, 3);

    vbox->addLayout(grid);

    selectKeySequence();
}

void KeyTraining::handleKeyPress(QChar letterPressed)
{
    if (letterPressed == '\r') {
        enterPressed();
        return;
    }

    if (m_keyCount >= m_required)
        return;

    if (m_keyCount == 0) {
        m_startTime.start();
        m_keyedTimes.push_back(0);
    } else {
        m_keyedTimes.push_back(m_startTime.elapsed());
    }

    m_keyCount++;

    if (m_keyCount == m_required) { // update the widgets based on the results
        m_timingDisplay->setTimings(m_keyedTimes, m_requiredTimes);
        calculateStats();
    }
}

void KeyTraining::handleKeyRelease(QChar letterPressed)
{
    handleKeyPress(letterPressed);  // doesn't really matter whether it's up/down
}

void KeyTraining::play()
{
    selectKeySequence();
    m_keyCount = 0;
    m_keyedTimes.clear();
    m_timingDisplay->clear();
    // pick new keying sequence, and display it
}

bool KeyTraining::enterPressed() {
    play();
    return true;
}

void KeyTraining::selectKeySequence()
{
    int currentMS = 0;
    m_requiredTimes.clear();

    // select the sequence we want them to key
    m_keySequence = "";
    for(int i = 0; i < m_letterCount; i++) {
        m_keySequence += completeCharacterSet[qrand() % completeCharacterSet.length()];
    }

    // translate the sequence into a list of timings (by MS)
    foreach(QChar letter, m_keySequence) {
        if (letter != ' ') {
            const QList<Morse::ditdah> *code = m_morse->getLetterCode(letter);
            // XXX: treat spaces specially (pop the last pause, add a longer one)
            foreach(Morse::ditdah segment, *code) {
                m_requiredTimes.push_back(currentMS);  // save the current spot as the beginning
                currentMS += ditDahToMS(segment);      // mark the end as the next spot
                m_requiredTimes.push_back(currentMS);  // save this as the end spot
                currentMS += m_morse->pauseSecsMS();   // add in the pause till the next letter
            }

            // at the end of the letter, change the sound pause to a letter pause
            currentMS += m_morse->letterPauseSecsMS() - m_morse->pauseSecsMS();
        } else {
            // we ended with a pause segment that needs to change to a space
            currentMS -= m_morse->pauseSecsMS();
            currentMS += m_morse->spaceSecsMS();
        }
    }

    m_required = m_requiredTimes.count();
    m_ui->letter->setText(m_keySequence);
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

void KeyTraining::calculateStats() {
    int expectedDits = 0, expectedDahs = 0;
    int keyedDits = 0, keyedDahs = 0;

    QList<int>::const_iterator requiredSpot = m_requiredTimes.begin();
    QList<int>::const_iterator keyedSpot = m_keyedTimes.begin();

    int startRequired;
    int startKeyed;
    int requiredDelta;
    int keyedDelta;

    // translate the sequence into a list of timings (by MS)
    foreach(QChar letter, m_keySequence) {
        if (letter != ' ') {
            const QList<Morse::ditdah> *code = m_morse->getLetterCode(letter);
            // XXX: treat spaces specially (pop the last pause, add a longer one)
            foreach(Morse::ditdah segment, *code) {

                // calculate the lengths of this spot
                startRequired = *requiredSpot;
                startKeyed = *keyedSpot;
                requiredSpot++;
                keyedSpot++;
                requiredDelta = *requiredSpot - startRequired;
                keyedDelta = *keyedSpot - startKeyed;

                switch(segment) {
                case Morse::DIT:
                    expectedDits += requiredDelta;
                    keyedDits += keyedDelta;
                    break;
                case Morse::DAH:
                    expectedDahs += requiredDelta;
                    keyedDahs += keyedDelta;
                    break;
                default:
                    qWarning() << "uh oh";
                    exit(98);
                }

                requiredSpot++;
                keyedSpot++;
                // XXX: calculate the pause spacing accuracy too
            }
            // XXX: calculate the letter pause accuracy
        } else {
            // XXX: calculate the space pausing
        }
    }

    // update all the labels
    if (expectedDits > keyedDits) {
        m_ditStats->setText(QString().setNum(100*(expectedDits - keyedDits)/expectedDits) + tr("% too fast"));
    } else {
        m_ditStats->setText(QString().setNum(100*(keyedDits - expectedDits)/keyedDits) + tr("% too slow"));
    }

    if (expectedDahs > keyedDahs) {
        m_dahStats->setText(QString().setNum(100*(expectedDahs - keyedDahs)/expectedDahs) + tr("% too fast"));
    } else {
        m_dahStats->setText(QString().setNum(100*(keyedDahs - expectedDahs)/keyedDahs) + tr("% too slow"));
    }
}
