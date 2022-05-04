#include "TrainingMode.h"
#include "Morse.h"

#include <qdebug.h>
#include <QMenu>
#include <QMenuBar>
#include <QProgressBar>

TrainingMode::TrainingMode(Morse *parent, Ui::MainWindow *ui)
    : MorseMode(parent, ui), MSequences(),
      m_doEntireSequence(false), m_maxBadLetters(2), m_includeProgressBars(true), m_acceptForwardKeys(true)
{
    setupSequences();
}

void TrainingMode::setupSequences() {
    m_trainingSequence = m_sequences[KOCH];

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

    // Similar
    subMenu = modeMenu->addMenu(tr("Similar"));

    action = subMenu->addAction("EISH5");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) SIM1);

    action = subMenu->addAction("TMO0");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) SIM2);

    action = subMenu->addAction("AUV4");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) SIM3);

    action = subMenu->addAction("NDB6");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) SIM4);

    action = subMenu->addAction("AWJ1");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) SIM5);


    // numbers and symbols

    action = modeMenu->addAction("Numbers");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) NUMBERS);

    action = modeMenu->addAction("Symbols");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) SYMBOLS);

    action = modeMenu->addAction("Everything");
    connect(action, SIGNAL(triggered()), m_sequenceSignalMapper, SLOT(map()));
    m_sequenceSignalMapper->setMapping(action, (int) EVERYTHING);

    // Custom

    action = modeMenu->addAction("Custom");
    connect(action, SIGNAL(triggered()), this, SLOT(chooseCustomeSequence()));

    // Connect the mapper
    connect(m_sequenceSignalMapper, SIGNAL(mapped(int)), this, SLOT(switchSequence(int)));
}

void TrainingMode::setupTrainingWidgets() {
    setupSequenceLayouts();

    m_ui->letter->setText("");
    m_ui->WPM->setText("");
}

void TrainingMode::modeMenus() {
    connect(menu()->addAction(tr("Clear Training")), SIGNAL(triggered()), this, SLOT(clear()));

    // Create the preference items in the quick menu
    m_doEntireSequenceButton = menu()->addAction(tr("Use &Entire Sequence"));
    m_doEntireSequenceButton->setCheckable(true);
    m_doEntireSequenceButton->setChecked(false);
    connect(m_doEntireSequenceButton, SIGNAL(toggled(bool)), this, SLOT(setDoEntireSequence(bool)));
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

    m_avewpmLabel->setText("All WPM: [None], K WPM: NEW");

    if (runningMode() == RUNNING)
        playButton(); // pretend a pause was pressed too
    updateGraphs();
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

    int msElapsed = lastTime.msecsTo(now) - m_morse->ditSecsF(); // subtract off blank-after time
    if (msElapsed <= 0)
        msElapsed = 1;
    qDebug() << "Training response: elapsed " << msElapsed << "ms (" << msToPauseWPM(msElapsed) << " WPM)";
    MorseStat *pressedStat = getStat(letterPressed);

    if (m_acceptForwardKeys && letterPressed != lastKey && m_lastKeys.count() > 0) {
        int whichLetter = 0;
        bool foundOne = false;

        foreach(QChar thekey, m_lastKeys) {
            if (letterPressed == thekey) {
                // We found the key they pressed that was correct.
                foundOne = true;
                break;
            }
            whichLetter++;
        }

        if (foundOne) {
            // drop (and penalize) all the earlier ones
            for(int i = 0; i < whichLetter; i++) {
                QChar theLetter = m_lastKeys.takeFirst();
                lastTime = m_lastTimes.takeFirst();
                getStat(theLetter)->addStat(3.0 * getStat(theLetter)->getAverageTime(), false);
                m_badCount++;
            }

            // penalize the key previously pulled off
            getStat(lastKey)->addStat(3.0 * getStat(lastKey)->getAverageTime(), false);
            m_badCount++;

            // pull the new matching key off
            lastKey = m_lastKeys.takeFirst();
            lastTime = m_lastTimes.takeFirst();
        }
    }

    // set the last WPM record on the display
    QString WPM = QString().setNum(msToPauseWPM(msElapsed)) + " ";
    foreach(QChar thechar, m_lastKeys) {
        WPM = WPM + "-";
    }
    qDebug() << "WPM text: " << WPM;
    m_lastwpmLabel->setText(WPM);

    // if the user took a *really* long time, ignore the key press and assume they got distracted from training
    if (elapsedTimeWasTooLong(msElapsed, pressedStat)) {
        qDebug() << "ignoring key press; too long and probably an interruption";
        return;
    }

    // if the keyed incorrectly, penalize them 3 times their average else add in the results
    if (letterPressed == lastKey) {
        pressedStat->addStat(msElapsed, true);
        m_goodCount++;
    } else {
        if (pressedStat->getTryCount() > 0) /* don't do this unless the letter has been pressed before */
            pressedStat->addStat(3.0 * pressedStat->getAverageTime(), false);
        if (getStat(lastKey)->getTryCount() > 0) /* don't do this unless the letter has been pressed before */
            getStat(lastKey)->addStat(3.0 * getStat(lastKey)->getAverageTime(), false);
        m_badCount++;
    }
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
    setupModeWidgets(m_trainingSequence);
}

void TrainingMode::setupModeWidgets(const QString &sequence, QString barLabel) {
    QVBoxLayout *vbox = new QVBoxLayout();
    clearLayout(m_ui->forModes);
    m_buttons = 0;
    m_ui->forModes->addLayout(vbox);
    setupKeyWidgets(sequence, vbox);
    setSequence(sequence, 1);
#ifndef SMALL_DEVICE
    if (m_includeProgressBars) {
        vbox->addLayout(setupGraphs(barLabel));
        updateGraphs();
    }
#endif
}

void TrainingMode::setSequence(const QString &sequence, int currentlyAt) {
    if (m_sequenceLabel) {
        QString left = sequence.left(currentlyAt);
        QString right = sequence.right(sequence.length() - currentlyAt);
        m_sequenceLabel->setText("<font color=\"red\">" + left.toUpper() + "</font>" + right.toUpper());

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

void TrainingMode::setupWidgets(const QString &sequence, bool includeProgressBars, QString barLabel)
{
    m_includeProgressBars = includeProgressBars;

    setupModeWidgets(sequence, barLabel);
}

QBoxLayout *TrainingMode::setupGraphs(QString barLabel)
{
    int column = 1;
    m_progressBars.clear();
    m_progressLabels.clear();
    QHBoxLayout *hbox = new QHBoxLayout();
    QGridLayout *gridLayout = new QGridLayout();

    QLabel *leftLabel = new QLabel(barLabel);
    leftLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    gridLayout->addWidget(leftLabel, 0, 0);

    foreach(QChar theLetter, m_trainingSequence) {
        QLabel *label = new QLabel(theLetter.toUpper());
        label->setAlignment(Qt::AlignCenter);
        gridLayout->addWidget(label, 1, column);

        GoalBar *bar = new GoalBar(0, 20);
        bar->setRange(0,20);
        bar->setValue(1);
        bar->setOrientation(Qt::Vertical);
        bar->setTextVisible(false);
        gridLayout->addWidget(bar, 0, column);
        column++;

        m_progressBars[theLetter] = bar;
        m_progressLabels[theLetter] = label;
    }
    hbox->addLayout(gridLayout);
    hbox->addStretch(1);
    return hbox;
}

void TrainingMode::updateGraphs()
{
}

bool TrainingMode::elapsedTimeWasTooLong(int msElapsed, MorseStat *stat) {
    Q_UNUSED(msElapsed);
    Q_UNUSED(stat);
    exit(1); // should not ever be called
}

void TrainingMode::saveStats(QSettings &settings, QString statPrefix) {
    if (statPrefix.length() == 0)
        statPrefix = name();
    foreach(QChar letter, m_sequences[EVERYTHING]) {
        m_stats[letter]->saveStats(settings, statPrefix + "/" + letter);
    }
}

void TrainingMode::loadStats(QSettings &settings, QString statPrefix) {
    if (statPrefix.length() == 0)
        statPrefix = name();
    foreach(QChar letter, m_sequences[EVERYTHING]) {
        getStat(letter)->loadStats(settings, statPrefix + "/" + letter);
    }
}
