#include "Morse.h"

#include <QtCore/QSettings>
#include <QtGui/QMenu>
#include <qdebug.h>

#include "MainWindow.h"

#define WPMGOAL   20
#define WPMACCEPT 15

Morse::Morse()
    : QObject(), m_sequenceLabel(0), m_parent(0), m_audioOutput(), m_dit(0), m_dah(0), m_space(0), m_pause(0), m_letterPause(0), m_playingMode(STOPPED), m_gameMode(PLAY),
    m_currentWPMGoal(WPMGOAL), m_currentWPMAccept(WPMACCEPT), m_statusBar(0), m_ui(0)
{
    qDebug() << "new morse";
    m_modes.insert(PLAY, new PlayMode(this, m_ui));
}

Morse::Morse(MainWindow *parent, QAudioOutput *output, Ui::MainWindow *ui)
    : QObject(parent), m_sequenceLabel(ui->sequence), m_parent(parent), m_audioOutput(output),
      m_dit(0), m_dah(0), m_space(0), m_pause(0), m_letterPause(0), m_playingMode(STOPPED), m_gameMode(PLAY),
      m_currentWPMGoal(WPMGOAL), m_currentWPMAccept(WPMACCEPT),
      m_statusBar(ui->status), m_ui(ui)
{

    qDebug() << "new morse2";
    createTones(WPMGOAL);
    setStatus("ready: Play Mode");
    qsrand(QTime::currentTime().msec());
    loadSettings();

    m_modes.insert(PLAY, new PlayMode(this, m_ui));
    m_modes.insert(TRAIN, new LetterTrainingMode(this, m_ui));
    m_modes.insert(SPEEDTRAIN, new SpeedTrainingMode(this, m_ui));
    m_modes.insert(WORDS, new WordTrainingMode(this, m_ui));
    m_modes.insert(READ, new ReadMode(this, m_ui));

    switchMode(Morse::PLAY);

    connect(m_ui->readButton, SIGNAL(clicked()), this, SLOT(readIt()));
    connect(m_ui->clearTraining, SIGNAL(clicked()), this, SLOT(clearStatsButton()));
    connect(m_ui->play, SIGNAL(clicked()), this, SLOT(playButton()));
}

void Morse::prefsButton() {
    Ui::Prefs prefsDialog;
    QDialog *dialog = new QDialog(m_parent);
    prefsDialog.setupUi(dialog);
    prefsDialog.WPMAccepted->setText(QString().setNum(m_currentWPMAccept));
    prefsDialog.WPMGoal->setText(QString().setNum(m_currentWPMGoal));

    prefsDialog.weighting->insertItem(LOW, "Low");
    prefsDialog.weighting->insertItem(HIGH, "High");
    prefsDialog.weighting->setCurrentIndex(m_badLetterWeighting);

    if (dialog->exec() == QDialog::Accepted) {
        m_currentWPMAccept = prefsDialog.WPMAccepted->text().toInt();
        m_currentWPMGoal = prefsDialog.WPMGoal->text().toInt();
        m_badLetterWeighting = (badLetterWeighting) prefsDialog.weighting->currentIndex();
        saveSettings();
        loadSettings();
    }
}

void Morse::saveSettings() {
    QSettings settings("WS6Z", "qtcw");
    settings.setValue("WPM/Goal", m_currentWPMGoal);
    settings.setValue("WPM/Accept", m_currentWPMAccept);
    settings.setValue("LetterWeighting", int(m_badLetterWeighting));
    qDebug() << "saving: " << m_badLetterWeighting;
}

void Morse::loadSettings() {
    QSettings settings("WS6Z", "qtcw");
    m_currentWPMGoal = settings.value("WPM/Goal", WPMGOAL).toInt();
    m_currentWPMAccept = settings.value("WPM/Accept", WPMACCEPT).toInt();
    m_badLetterWeighting = (badLetterWeighting) settings.value("LetterWeighting", HIGH).toInt();
    createTones(m_currentWPMGoal);  
}

void Morse::clearStatsButton() {
    m_modes[m_gameMode]->clear();
}

void
Morse::playSequence()
{
    qDebug() << "Playing!";
    m_playBuffer->restartData();
    m_playBuffer->start();
    m_playingMode = PLAYING;
    m_audioOutput->start(m_playBuffer);
    return;
}

void Morse::maybePlaySequence() {
    qDebug() << "maybe going to key: " << " / " << m_playingMode;
    if (m_playingMode == STOPPED || m_playingMode == PAUSED) {
        qDebug() << "going to key: ";
        playSequence();
    }
}


void Morse::keyPressed(QString newtext) {
    QChar newletter = newtext.at(newtext.length()-1).toLower();
    // qDebug() << "user pressed: " << newletter << "and last key was: " << m_lastKey;
    keyPressed(newletter);
}

bool Morse::enterPressed() {
    return m_modes[m_gameMode]->enterPressed();
}

Morse::playingmode Morse::playingMode() {
    return m_playingMode;
}

void Morse::playButton() {
    m_modes[m_gameMode]->playButton();
}

void Morse::keyPressed(QChar newletter) {
    m_modes[m_gameMode]->handleKeyPress(newletter);
}

MorseStat *Morse::getStat(const QChar &key) {
    if (! m_stats.contains(key))
        m_stats[key] = new MorseStat(0);
    return m_stats[key];
}


void Morse::generatorDone() {
    //qDebug() << "generator says done";
    audioFinished(QAudio::StoppedState); // fixes windows issues
}

void Morse::setPlayingMode(playingmode newmode) {
    m_playingMode = newmode;
}

Morse::mode Morse::gameMode() {
    return m_gameMode;
}

Morse::badLetterWeighting Morse::get_badLetterWeighting() {
    return m_badLetterWeighting;
}

void
Morse::audioFinished(QAudio::State state)
{
    m_modes[m_gameMode]->audioFinished(state);
}

void Morse::switchMode(int newmode) {
    m_gameMode = (Morse::mode) newmode;
    qDebug() << "switch to:" << m_gameMode;
    m_playBuffer->stop();
    m_ui->letter->setText("");
    m_ui->WPM->setText("");

    m_modes[(mode) newmode]->switchToMode();
}

//
// HERE and below is tone generation and sequence playing
//

void Morse::addAndPlayIt(QChar c) {
    if (m_playingMode == STOPPED || m_playingMode == PAUSED) {
        clearList();
        add(pause());
    }
    add(c, false);
    add(m_letterPause);
    maybePlaySequence();
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
        add(*letter);
    }
}

void
Morse::createTones(int wpm)
{
    createTones(float(60.0/float(wpm*50.0)));
}

void
Morse::createTones(float ditSecs, int dahMult, int pauseMult, int letterPauseMult, int spaceMult)
{
    m_ditSecs = ditSecs;

    m_dit = new Generator(ditSecs);
    m_dit->start();

    m_dah = new Generator(ditSecs * dahMult);
    m_dah->start();

    m_pause = new Generator(ditSecs * pauseMult, 0);
    m_pause->start();

    m_letterPause = new Generator(ditSecs * letterPauseMult, 0);
    m_letterPause->start();

    m_space = new Generator(ditSecs * spaceMult, 0);
    m_space->start();

    m_playBuffer = new Generator(m_pause);
    m_playBuffer->start();
    connect(m_playBuffer, SIGNAL(generatorDone()), this, SLOT(generatorDone()));

    #include "morse_code.h"

    qDebug() << "created tones";
    connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(audioFinished(QAudio::State)));
}

void Morse::setStatus(const QString &status) {
    if (m_statusBar)
        m_statusBar->setText(status);
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
