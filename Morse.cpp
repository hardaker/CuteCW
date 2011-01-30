#include "Morse.h"

#include <QtCore/QSettings>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <qdebug.h>

#include "MainWindow.h"

#include "modes/PlayMode.h"
#include "modes/LetterTrainingMode.h"
#include "modes/SpeedTrainingMode.h"
#include "modes/WordTrainingMode.h"
#include "modes/ReadMode.h"
#include "modes/GroupingMode.h"
#include "modes/games/WordGame.h"
#include "modes/games/GroupGame.h"

#define WPMGOAL   20
#define WPMACCEPT 2

Morse::Morse()
    : QObject(), m_sequenceLabel(0), m_parent(0), m_audioOutput(),
      m_dahMult(3), m_pauseMult(1), m_letterPauseMult(3), m_spaceMult(7),
      m_dit(0), m_dah(0), m_space(0), m_pause(0), m_letterPause(0), m_playBuffer(0), m_playingMode(STOPPED), m_gameMode(PLAY),
    m_currentWPMGoal(WPMGOAL), m_currentWPMAccept(WPMACCEPT), m_ui(0), m_tone(DEFAULT_TONE)
{
    qDebug() << "new morse";
    m_modes.insert(PLAY, new PlayMode(this, m_ui));
#include "morse_code.h"
}

Morse::Morse(MainWindow *parent, QAudioOutput *output, Ui::MainWindow *ui)
    : QObject(parent), m_sequenceLabel(ui->sequence), m_parent(parent), m_audioOutput(output),
      m_dahMult(3), m_pauseMult(1), m_letterPauseMult(3), m_spaceMult(7),
      m_dit(0), m_dah(0), m_space(0), m_pause(0), m_letterPause(0), m_playBuffer(0), m_playingMode(STOPPED), m_gameMode(PLAY),
      m_currentWPMGoal(WPMGOAL), m_currentWPMAccept(WPMACCEPT),
      m_ui(ui), m_tone(DEFAULT_TONE)
{

    qDebug() << "new morse2";

#include "morse_code.h"

    createTones(WPMGOAL);
    qsrand(QTime::currentTime().msec());
    loadSettings();

    m_modes.insert(PLAY, new PlayMode(this, m_ui));
    m_modes.insert(TRAIN, new LetterTrainingMode(this, m_ui));
    m_modes.insert(SPEEDTRAIN, new SpeedTrainingMode(this, m_ui));
    m_modes.insert(WORDS, new WordTrainingMode(this, m_ui));
    m_modes.insert(GROUPS, new GroupingMode(this, m_ui));
    m_modes.insert(WORDGAME, new WordGame(this, m_ui));
    m_modes.insert(GROUPGAME, new GroupGame(this, m_ui));
    m_modes.insert(READ, new ReadMode(this, m_ui));

    switchMode(Morse::PLAY);
}

void Morse::prefsButton() {
    Ui::Prefs prefsDialog;
    QDialog *dialog = new QDialog(m_parent);
    prefsDialog.setupUi(dialog);
    prefsDialog.WPMAccepted->setValue(m_currentWPMAccept);
    prefsDialog.WPMGoal->setValue(m_currentWPMGoal);
    prefsDialog.tone->setValue(m_tone);

    prefsDialog.weighting->insertItem(LOW, "Low");
    prefsDialog.weighting->insertItem(HIGH, "High");
    prefsDialog.weighting->setCurrentIndex(m_badLetterWeighting);

    if (dialog->exec() == QDialog::Accepted) {
        m_currentWPMAccept = prefsDialog.WPMAccepted->value();
        m_currentWPMGoal = prefsDialog.WPMGoal->value();
        m_badLetterWeighting = (BadLetterWeighting) prefsDialog.weighting->currentIndex();
        m_tone = prefsDialog.tone->value();
        saveSettings();
        loadSettings();
    }
}

void Morse::aboutButton() {
    Ui::AboutDialog about;
    QDialog *dialog = new QDialog(m_parent);
    about.setupUi(dialog);
    about.helpicon->setPixmap(QPixmap(":/icons/64x64/cutecw.png"));
    dialog->exec();
}

QMenuBar *
Morse::menuBar()
{
    return m_parent->menuBar();
}

void Morse::saveSettings() {
    QSettings settings("WS6Z", "qtcw");
    settings.setValue("WPM/Goal", m_currentWPMGoal);
    settings.setValue("WPM/Accept", m_currentWPMAccept);
    settings.setValue("Tone", m_tone);
    settings.setValue("LetterWeighting", int(m_badLetterWeighting));
    qDebug() << "saving: " << m_badLetterWeighting;
}

void Morse::loadSettings() {
    QSettings settings("WS6Z", "qtcw");
    m_currentWPMGoal = settings.value("WPM/Goal", WPMGOAL).toInt();
    m_currentWPMAccept = settings.value("WPM/Accept", WPMACCEPT).toInt();
    m_badLetterWeighting = (BadLetterWeighting) settings.value("LetterWeighting", HIGH).toInt();
    m_tone = settings.value("Tone", DEFAULT_TONE).toInt();
    createTones(m_currentWPMGoal);  
}

void Morse::clearStatsButton() {
    m_modes[m_gameMode]->clear();
}

void
Morse::playSequence()
{
    m_playBuffer->restartData();
    m_playBuffer->start();
    m_playingMode = PLAYING;
    m_audioOutput->start(m_playBuffer);
    return;
}

QTime Morse::sequenceTime() {
    return m_playBuffer->timeLeft();
}

QTime Morse::maybePlaySequence(bool addPause) {
    if (m_playingMode == STOPPED || m_playingMode == PAUSED) {
        m_playBuffer->restartData();
        QTime playTime = sequenceTime();
        playSequence();
        if (addPause)
            add(pause());
        return QTime::currentTime().addSecs(playTime.second()).addMSecs(playTime.msec());
    }
    return QTime(0,0,0);
}


void Morse::keyPressed(QString newtext) {
    QChar newletter = newtext.at(newtext.length()-1).toLower();
    qDebug() << "*** user pressed: " << newletter;
    keyPressed(newletter);
}

bool Morse::enterPressed() {
    return m_modes[m_gameMode]->enterPressed();
}

Morse::AudioMode Morse::audioMode() {
    return m_playingMode;
}

void Morse::playButton() {
    m_modes[m_gameMode]->playButton();
}

void Morse::keyPressed(QChar newletter) {
    m_modes[m_gameMode]->handleKeyPress(newletter);
}

void Morse::generatorDone() {
    audioFinished(QAudio::StoppedState); // fixes windows issues
}

void Morse::setAudioMode(AudioMode newmode) {
    m_playingMode = newmode;
}

void Morse::pauseAudio() {
    setAudioMode(PAUSED);
}

void Morse::playAudio() {
    setAudioMode(PLAYING);
}

Morse::TrainingMode Morse::trainingMode() {
    return m_gameMode;
}

Morse::BadLetterWeighting Morse::badLetterWeighting() {
    return m_badLetterWeighting;
}

void
Morse::audioFinished(QAudio::State state)
{
    m_modes[m_gameMode]->audioFinished(state);
}

void Morse::switchMode(int newmode) {
    qDebug() << "switch to:" << newmode;
    m_playBuffer->stop();
    m_ui->letter->setText("");
    m_ui->WPM->setText("");

    m_modes[m_gameMode]->switchFromYou();
    m_gameMode = (Morse::TrainingMode) newmode;
    setupWidgets();
    m_modes[(TrainingMode) newmode]->switchToYou();

    connect(menuBar()->addAction("About"), SIGNAL(triggered()), this, SLOT(aboutButton()));
}

//
// HERE and below is tone generation and sequence playing
//

QTime Morse::addAndPlayIt(QChar c) {
    if (m_playingMode == STOPPED || m_playingMode == PAUSED) {
        clearList();
        add(pause());
    }
    add(c, false);
    add(m_letterPause);
    return maybePlaySequence();
}

QTime Morse::playIt(QChar c) {
    clearList();
    add(pause());  // allows audio device to kick in (otherwise distortion can occur)
    add(c, false);
    return maybePlaySequence(true);
}

void
Morse::clearList()
{
    m_playBuffer->clearBuffer();
}

void
Morse::add(Generator *nextSound)
{
    m_playBuffer->appendDataFrom(nextSound);
}

void
Morse::add(QChar c, bool addpause)
{
    c = c.toLower();
    if (! code.contains(c))
        return;

    QList<ditdah>::iterator iter;
    QList<ditdah>::iterator endat = code[c]->end();

    bool lastWasPause = false;
    for(iter = code[c]->begin(); iter != endat; iter++)
    {
        lastWasPause = false;
        switch (*iter) {
        case DIT:
            add(m_dit);
            break;
        case DAH:
            add(m_dah);
            break;
        case PAUSE:
            add(m_pause);
            lastWasPause = true;
            break;
        case SPACE:
            add(m_space);
            lastWasPause = true;
            break;
        default:
            qWarning() << "error: illegal morse type added";
        }
        add(m_pause);
    }
    if (addpause && !lastWasPause) {
        add(m_letterPause);
    }
}

void Morse::add(const QString &textToAdd) {
    QString::const_iterator letter;
    QString::const_iterator lastLetter = textToAdd.end();

    clearList();
    for (letter = textToAdd.begin(); letter != lastLetter; ++letter) {
        add(*letter, true);
    }
}

void
Morse::createTones(int wpm, int spacewpm, int letterspacewpm)
{
    if (spacewpm == -1)
        spacewpm = wpm;
    if (letterspacewpm == -1)
        letterspacewpm = spacewpm;
    createSpacedTones(float(60.0/float(wpm*50.0)), float(float(m_spaceMult) * float(60.0/float(spacewpm*50.0))),
                float(float(m_letterPauseMult) * float(60.0/float(letterspacewpm*50.0))));
}

void
Morse::_createTones()
{
    if (m_dit)
        delete m_dit;
    m_dit = new Generator(m_ditSecs, m_tone);
    m_dit->start();

    if (m_dah)
        delete m_dah;
    m_dah = new Generator(m_dahSecs, m_tone);
    m_dah->start();

    if (m_pause)
        delete m_pause;
    m_pause = new Generator(m_pauseSecs, 0);
    m_pause->start();

    if (m_letterPause)
        delete m_letterPause;
    m_letterPause = new Generator(m_letterPauseSecs, 0);
    m_letterPause->start();

    if (m_space)
        delete m_space;
    m_space = new Generator(m_spaceSecs, 0);
    m_space->start();

    if (!m_playBuffer) {
        m_playBuffer = new Generator(m_pause);
        m_playBuffer->start();
        connect(m_playBuffer, SIGNAL(generatorDone()), this, SLOT(generatorDone()), Qt::QueuedConnection);
    }

    connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(audioFinished(QAudio::State)));
}

void
Morse::createTones(float ditSecs, int dahMult, int pauseMult, int letterPauseMult, int spaceMult)
{
    m_ditSecs = ditSecs;
    m_dahSecs = ditSecs * dahMult;
    m_pauseSecs = ditSecs * pauseMult;
    m_letterPauseSecs = ditSecs * letterPauseMult;
    m_spaceSecs = ditSecs * spaceMult;

    m_dahMult = dahMult;
    m_pauseMult = pauseMult;
    m_letterPauseMult = letterPauseMult;
    m_spaceMult = spaceMult;

    _createTones();
}

void
Morse::createSpacedTones(float ditSecs, float spaceSecs, float letterSpaceSecs)
{
    m_ditSecs = ditSecs;
    m_spaceSecs = spaceSecs;
    m_letterPauseSecs = letterSpaceSecs;
    m_dahSecs = ditSecs * m_dahMult;
    m_pauseSecs = ditSecs * m_pauseMult;
    _createTones();
}

int Morse::currentWPMAccept() {
    return m_currentWPMAccept;
}

int Morse::currentWPMGoal() {
    return m_currentWPMGoal;
}

Generator *
Morse::dit()
{
    return m_dit;
}

Generator *
Morse::dah()
{
    return m_dah;
}

Generator *
Morse::pause()
{
    return m_pause;
}

Generator *
Morse::letterPause()
{
    return m_letterPause;
}

Generator *
Morse::space()
{
    return m_space;
}

float
Morse::ditSecs()
{
    return m_ditSecs;
}

void Morse::setupWidgets()
{
    QWidget *theMainThing = m_ui->centralWidget = new QWidget(m_parent);
    QVBoxLayout *topvbox = m_ui->verticalLayout = new QVBoxLayout(theMainThing);

    QHBoxLayout *buttonHBox = m_ui->horizontalLayout = new QHBoxLayout(theMainThing);
    topvbox->addLayout(buttonHBox);

    setupTopButtons(buttonHBox);
    topvbox->addWidget(m_ui->helpBar = new QLabel("help"));
    setupWPMLayout(topvbox, theMainThing);
    topvbox->addLayout(m_ui->forModes = new QHBoxLayout(theMainThing));
    setupSequenceLayouts(topvbox, theMainThing);

    setupConnections();

    m_parent->setCentralWidget(theMainThing);
}


void Morse::setupWPMLayout(QVBoxLayout *parentLayout, QWidget *theMainThing)
{
    QHBoxLayout *WPMLayout = m_ui->horizontalLayout_5 = new QHBoxLayout(theMainThing);
    parentLayout->addLayout(WPMLayout);

    WPMLayout->addWidget(m_ui->WPM = new QLabel(""));
    WPMLayout->addWidget(m_ui->letter = new QLabel(""));

    QFont font = m_ui->WPM->font();
    font.setPointSize(font.pointSize() * 2); // A cheap way of getting bigger regardless of the default stylee
    font.setBold(true);
    m_ui->WPM->setFont(font);

    font = m_ui->letter->font();
    font.setPointSize(font.pointSize() * 2);
    font.setBold(true);
    m_ui->letter->setFont(font);
}

void Morse::setupSequenceLayouts(QVBoxLayout *parentLayout, QWidget *theMainThing)
{
    QHBoxLayout *sequenceLayout = m_ui->horizontalLayout_3 = new QHBoxLayout(theMainThing);
    parentLayout->addLayout(sequenceLayout);
    sequenceLayout->addWidget(m_ui->label = new QLabel(tr("Sequence:")));
    sequenceLayout->addWidget(m_sequenceLabel = m_ui->sequence = new QLabel(tr("")));

    QHBoxLayout *lastWPMLayout = m_ui->horizontalLayout_4 = new QHBoxLayout(theMainThing);
    parentLayout->addLayout(lastWPMLayout);
    lastWPMLayout->addWidget(m_ui->label_3 = new QLabel(tr("Last WPM:")));
    lastWPMLayout->addWidget(m_ui->lastwpm = new QLabel(tr("")));

    QHBoxLayout *aveWPMLayout = m_ui->horizontalLayout_3 = new QHBoxLayout(theMainThing);
    parentLayout->addLayout(aveWPMLayout);
    aveWPMLayout->addWidget(m_ui->label_3 = new QLabel(tr("Average WPM:")));
    aveWPMLayout->addWidget(m_ui->avewpm = new QLabel(tr("")));
}

void Morse::setupTopButtons(QLayout *parentLayout)
{
    QPushButton *button;

    button = m_ui->modeMenu = new QPushButton(tr("Mode"));
    parentLayout->addWidget(button);
    createModesMenu(button);

    button = m_ui->play = new QPushButton(tr("Play"));
    parentLayout->addWidget(button);

    button = m_ui->changeSequence = new QPushButton(tr("Sequence"));
    parentLayout->addWidget(button);

    button = m_ui->changeWords = new QPushButton(tr("Change Words"));
    parentLayout->addWidget(button);

    button = m_ui->clearTraining = new QPushButton(tr("Clear Training"));
    parentLayout->addWidget(button);

    button = m_ui->prefs = new QPushButton(tr("Preferences"));
    parentLayout->addWidget(button);
}

void Morse::createModesMenu(QPushButton *modeButton) {
    // setup mode menu
    m_signalMapper = new QSignalMapper(this);

    // Create the "mode" menu
    QMenu *modeMenu = new QMenu(modeButton);
    modeButton->setMenu(modeMenu);

    QAction *action = modeMenu->addAction(tr("Type Morse Code"));
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::PLAY);

    QMenu *trainingMenu = modeMenu->addMenu(tr("Training"));

    action = trainingMenu->addAction(tr("Recognition Train"));
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::TRAIN);

    action = trainingMenu->addAction(tr("Speed Training"));
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::SPEEDTRAIN);

    action = trainingMenu->addAction(tr("Word Training"));
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::WORDS);

    action = trainingMenu->addAction(tr("Grouping Training"));
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::GROUPS);

    QMenu *gamesMenu = modeMenu->addMenu(tr("Games"));

    action = gamesMenu->addAction(tr("Word Accuracy"));
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::WORDGAME);

    action = gamesMenu->addAction(tr("Grouping Accuracy"));
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::GROUPGAME);

    action = modeMenu->addAction(tr("Read to me!"));
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::READ);

    connect(m_signalMapper, SIGNAL(mapped(int)), this, SLOT(switchMode(int)));
    m_ui->modeMenu->setText(tr("Type Morse Code"));
}

void Morse::setupConnections()
{
    connect(m_ui->prefs, SIGNAL(clicked()), this, SLOT(prefsButton()));

    connect(m_ui->clearTraining, SIGNAL(clicked()), this, SLOT(clearStatsButton()));
    connect(m_ui->play, SIGNAL(clicked()), this, SLOT(playButton()));
}
