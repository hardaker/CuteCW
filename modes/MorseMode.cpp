#include "MorseMode.h"
#include "Morse.h"
#include "MainWindow.h"

#include <QMenuBar>
#include <QMessageBox>
#include <QTextEdit>
#include <QSpinBox>
#include <QLabel>
#include <QScrollArea>
#include <QHBoxLayout>
#include <qdebug.h>

MorseMode::MorseMode(Morse *morse, Ui::MainWindow *ui)
    : m_morse(morse), m_ui(ui),
      m_badCount(0), m_goodCount(0),
      m_playIcon(":/icons/play.png"), m_pauseIcon(":/icons/pause.png"), m_runningMode(PAUSED),
      m_mapper(new QSignalMapper()), m_buttons(0), m_modeSpecificMenu(0)
{
    setRunningMode(PAUSED);
    m_WPM = m_morse->currentWPMGoal();
    m_spaceWPM = m_WPM;
    m_letterSpaceWPM = m_WPM;
    m_morse->createTones(m_WPM, m_spaceWPM, m_letterSpaceWPM);
}

Morse *MorseMode::morseParent() {
    return m_morse;
}

void MorseMode::playButton() {
    qDebug() << "playButton()";

    if (m_runningMode == RUNNING) {
        // stop! (pause, whatever...)
        setRunningMode(PAUSED);
        m_morse->setAudioMode(Morse::STOPPED);
    } else {
        setRunningMode(RUNNING);
    }
}

void MorseMode::play()
{
}

void MorseMode::stop()
{
}

void MorseMode::help() {
    QScrollArea *scrolled = new QScrollArea();
    QTextEdit *helptext = new QTextEdit();
    helptext->setReadOnly(true);
    helptext->append("<h2>" + name() + " Mode</h2>\n" + helpText());
    scrolled->setWidget(helptext);
    scrolled->setWidgetResizable(true);
    scrolled->resize(800,440);
#ifdef SMALL_DEVICE
    scrolled->setAttribute(Qt::WA_Maemo5StackedWindow);
#endif
    scrolled->setWindowModality(Qt::WindowModal);
    scrolled->show();
}

bool MorseMode::enterPressed() {
    return false;
}

void MorseMode::handleKeyPress(QChar letterPressed) {
    Q_UNUSED(letterPressed);
}

void MorseMode::handleKeyPress(const QString &letterPressed) {
    handleKeyPress(letterPressed[0]);
}

void MorseMode::handleKeyRelease(QChar letterReleased) {
    Q_UNUSED(letterReleased);
}

void MorseMode::handleKeyRelease(const QString &letterReleased) {
    handleKeyRelease(letterReleased[0]);
}

void MorseMode::audioFinished(QAudio::State state) {
    if (state != QAudio::IdleState && state != QAudio::StoppedState)
        return;

    // qDebug() << "audio state changed: " << state << ", old state = " << m_morse->audioMode();

    if (m_morse->audioMode() != Morse::STOPPED) {
        audioStopped();
    }
    m_morse->setAudioMode(Morse::STOPPED);
}

void MorseMode::audioStopped() {
}

void MorseMode::clear() {
}


// Utilities

int MorseMode::msToWPM(float ms) {
    return (60*1000)/ms;
}

int MorseMode::msToPauseWPM(float ms) {
    return int(msToPauseWPMF(ms));
}

float MorseMode::msToPauseWPMF(float ms) {
    // 3 dits in length is the pause between letter spacing
    float pauseLength = 3.0 * m_morse->ditSecsF();
    // calculate the WPM based on the space it took for the letter to be identified during the pause
    // qDebug() << "pause length: " << pauseLength << ", recorded time: " << ms << ", % = " << (pauseLength * 1000.0 * 100.0 / ms );
    return float(m_morse->currentWPMGoal()) * pauseLength * 1000.0 / ms;
}

void MorseMode::hideWidgets()
{
    m_ui->letter->hide();
    m_ui->changeSequence->hide();
    m_ui->changeWords->hide();
    m_ui->play->hide();
    m_ui->WPM->hide();
#ifdef SMALL_DEVICE
    m_ui->helpBar->hide();
#endif
    clearLayout(m_ui->forModes);
    //m_ui->forModes->hide();
}

void MorseMode::switchToYou()
{
    createMenuStructures();
    m_morse->createTones(m_morse->currentWPMGoal());
    hideWidgets();
    switchToMode();
    createGlobalActions();
}

void MorseMode::createMenuStructures()
{
#ifdef SMALL_DEVICE
    m_helpMenu = m_optionsMenu = m_cuteCWMenu = m_modeSpecificMenu = m_morse->menuBar();
    // m_morse->createModesMenu(m_morse->menuBar()->addMenu(tr("Change Mode")));
#else
    QMenuBar *topBar = m_morse->menuBar();
    m_cuteCWMenu = topBar->addMenu("&CuteCW");
    m_morse->createModesMenu(m_cuteCWMenu->addMenu(tr("Change Mode")));
    m_optionsMenu = topBar->addMenu(tr("&Options"));
    modeMenus();
    m_helpMenu = topBar->addMenu(tr("&Help"));
#endif
}

#ifdef SMALL_DEVICE
QMenuBar *MorseMode::menu() {
    return m_morse->menuBar();
}
#else
QMenu *MorseMode::menu() {
    if (!m_modeSpecificMenu)
        m_modeSpecificMenu = m_morse->menuBar()->addMenu(name());

    return m_modeSpecificMenu;
}
#endif

void MorseMode::modeMenus()
{
}

void MorseMode::createGlobalActions()
{
    // Create the preference items in the quick menu
    QAction *button = m_helpMenu->addAction(tr("&Mode Help"));
    connect(button, SIGNAL(triggered()), this, SLOT(help()));

    connect(m_helpMenu->addAction(tr("&About")), SIGNAL(triggered()), m_morse, SLOT(aboutButton()));

    connect(m_optionsMenu->addAction(tr("&Preferences")), SIGNAL(triggered()), m_morse, SLOT(prefsButton()));

    connect(m_cuteCWMenu->addAction(tr("&Quit")), SIGNAL(triggered()), m_morse->parent(), SLOT(close()));
}

void MorseMode::clearModeLayout() {
    // remove the mode specific layout objects
    clearLayout(m_ui->forModes);
    m_buttons = 0;
}

void MorseMode::switchFromYou()
{
    // let the mode clean itself up first
    switchFromMode();

    // erase the menu and mode layout
    m_morse->menuBar()->clear();
    clearModeLayout();

    // clear the mode specific menu
    if (m_modeSpecificMenu) {
        m_modeSpecificMenu->clear();
        delete m_modeSpecificMenu;
        m_modeSpecificMenu = 0;
    }

    // stop the audio
    m_morse->pauseAudio();
    setRunningMode(PAUSED);

}

void MorseMode::switchFromMode()
{
}

MorseMode::RunningMode MorseMode::runningMode()
{
    return m_runningMode;
}

void MorseMode::setRunningMode(RunningMode newMode)
{
    m_runningMode = newMode;
    if (m_runningMode == RUNNING) {
        m_ui->play->setIcon(m_pauseIcon);
        m_ui->play->setText(tr("Pause"));
        play();
    } else {
        if (m_morse->audioMode() != Morse::STOPPED) {
            m_morse->pauseAudio();
        }
        m_ui->play->setIcon(m_playIcon);
        m_ui->play->setText(tr("Play"));
        stop();
    }
}

void MorseMode::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        // XXX delete item;
    }
}

void MorseMode::setupKeyWidgets(const QString &sequence, QBoxLayout *inside) {
    qDebug() << "setting up sequence buttons";

    // if we don't have a grid yet, create it
    if (m_buttons) {
        clearLayout(m_buttons);
        delete m_buttons;
        m_buttons = 0;
    }

    m_buttons = new QGridLayout();
    if (inside)
        inside->addLayout(m_buttons);
    else
        m_ui->forModes->addLayout(m_buttons);

    int column = 0;
    int row = 0;
    const int buttonsPerRow = 11;
    foreach (QChar letter, sequence) {
        QPushButton *button = new QPushButton(QString(letter));
        m_buttons->addWidget(button, row, column++);
        connect(button, SIGNAL(clicked()), m_mapper, SLOT(map()));
        m_mapper->setMapping(button, letter);
        if (column == buttonsPerRow) {
            column = 0;
            row++;
        }

    }
    connect(m_mapper, SIGNAL(mapped(const QString &)),
            this, SLOT(handleKeyPress(const QString &)));
}

void MorseMode::setupWPMWidgets(QBoxLayout *to, WPMWidgets whichWidgets) {
    if (!to)
        to = m_ui->forModes;

    QVBoxLayout *vbox = new QVBoxLayout();
    to->addLayout(vbox);

    QLabel *label;
    QSpinBox *spinbox;
    QHBoxLayout *hbox;

    if (whichWidgets & WPMRATE) {
        hbox = new QHBoxLayout();
        vbox->addLayout(hbox);

        label = new QLabel(tr("WPM:"));
        hbox->addWidget(label);

        spinbox = new QSpinBox();
        spinbox->setMaximum(100);
        spinbox->setMinimum(1);
        spinbox->setValue(m_WPM);
        hbox->addWidget(spinbox);
        connect(spinbox, SIGNAL(valueChanged(int)), this, SLOT(changeWPM(int)));
        changeWPM(m_WPM);
    }

    if (whichWidgets & LETTERRATE) {
        hbox = new QHBoxLayout();
        vbox->addLayout(hbox);

        label = new QLabel(tr("Letter Spacing WPM:"));
        hbox->addWidget(label);

        spinbox = new QSpinBox();
        spinbox->setMaximum(100);
        spinbox->setMinimum(1);
        spinbox->setValue(m_letterSpaceWPM);
        hbox->addWidget(spinbox);
        connect(spinbox, SIGNAL(valueChanged(int)), this, SLOT(changeLetterSpaceWPM(int)));
        changeLetterSpaceWPM(m_letterSpaceWPM);
    }

    if (whichWidgets & WORDRATE) {
        hbox = new QHBoxLayout();
        vbox->addLayout(hbox);

        label = new QLabel(tr("Word Spacing WPM:"));
        hbox->addWidget(label);

        spinbox = new QSpinBox();
        spinbox->setMaximum(100);
        spinbox->setMinimum(1);
        spinbox->setValue(m_spaceWPM);
        hbox->addWidget(spinbox);
        connect(spinbox, SIGNAL(valueChanged(int)), this, SLOT(changeSpaceWPM(int)));
        changeSpaceWPM(m_spaceWPM);
    }
}

void MorseMode::changeWPM(int wpm) {
    m_WPM = wpm;
    m_morse->createTones(m_WPM, m_spaceWPM, m_letterSpaceWPM);
}

void MorseMode::changeSpaceWPM(int wpm) {
    m_spaceWPM = wpm;
    m_morse->createTones(m_WPM, m_spaceWPM, m_letterSpaceWPM);
}


void MorseMode::changeLetterSpaceWPM(int wpm) {
    m_letterSpaceWPM = wpm;
    m_morse->createTones(m_WPM, m_spaceWPM, m_letterSpaceWPM);
}

void MorseMode::saveSettings(QSettings &settings) {
    Q_UNUSED(settings);
}

void MorseMode::loadSettings(QSettings &settings) {
    Q_UNUSED(settings);
}

void MorseMode::setupSequenceLayout(QVBoxLayout *parentLayout, QWidget *theMainThing)
{
    if (parentLayout == 0)
        parentLayout = m_ui->verticalLayout;
    if (theMainThing == 0)
        theMainThing = m_ui->centralWidget;

    m_sequenceLayout = new QHBoxLayout(theMainThing);
    parentLayout->addLayout(m_sequenceLayout);
    m_sequenceLayout->addWidget(new QLabel(tr("Sequence:")));
    m_sequenceLayout->addWidget(m_sequenceLabel = new QLabel(tr("")));
}

void MorseMode::setupLastWPMLayout(QVBoxLayout *parentLayout, QWidget *theMainThing)
{
    if (parentLayout == 0)
        parentLayout = m_ui->verticalLayout;
    if (theMainThing == 0)
        theMainThing = m_ui->centralWidget;

    m_lastWPMLayout = new QHBoxLayout(theMainThing);
    parentLayout->addLayout(m_lastWPMLayout);
    m_lastWPMLayout->addWidget(new QLabel(tr("Last WPM:")));
    m_lastWPMLayout->addWidget(m_lastwpmLabel = new QLabel(tr("")));
}

void MorseMode::setupAveWPMLayout(QVBoxLayout *parentLayout, QWidget *theMainThing)
{
    if (parentLayout == 0)
        parentLayout = m_ui->verticalLayout;
    if (theMainThing == 0)
        theMainThing = m_ui->centralWidget;

    m_aveWPMLayout = new QHBoxLayout(theMainThing);
    parentLayout->addLayout(m_aveWPMLayout);
    m_aveWPMLayout->addWidget(new QLabel(tr("Average WPM:")));
    m_aveWPMLayout->addWidget(m_avewpmLabel = new QLabel(tr("")));
}

void MorseMode::setupSequenceLayouts(QVBoxLayout *parentLayout, QWidget *theMainThing)
{
    setupSequenceLayout(parentLayout, theMainThing);
    setupLastWPMLayout(parentLayout, theMainThing);
    setupAveWPMLayout(parentLayout, theMainThing);
}

QBoxLayout *MorseMode::getPrefsLayout()
{
    return 0;
}

void MorseMode::acceptPrefs()
{
}

void MorseMode::rejectPrefs()
{
}

QString MorseMode::icon()
{
    return QString();
}
