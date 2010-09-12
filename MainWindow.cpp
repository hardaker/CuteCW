#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ui_Prefs.h"
#include <QtMultimedia/QAudioFormat>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_audioOutput(0),
    m_morse(0)

{
    ui->setupUi(this);

    // setup mode menu
    m_signalMapper = new QSignalMapper(this);

    // Create the "mode" menu
    QMenu *modeMenu = new QMenu(ui->modeMenu);
    ui->modeMenu->setMenu(modeMenu);

    QAction *action = modeMenu->addAction("Play Morse Code");
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::PLAY);

    action = modeMenu->addAction("Recognition Train");
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::TRAIN);

    action = modeMenu->addAction("Speed Training");
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::SPEEDTRAIN);

    action = modeMenu->addAction("Word Training");
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::WORDS);

    action = modeMenu->addAction("Read");
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::READ);

    // Create the preference items in the quick menu
    action = menuBar()->addAction("Use Entire Sequence");
    action->setCheckable(true);
    action->setChecked(false);
    connect(action, SIGNAL(toggled(bool)), m_morse, SLOT(setDoEntireSequence(bool)));


    createAudioOutput();
    connect(ui->prefs, SIGNAL(clicked()), m_morse, SLOT(prefsButton()));
    ui->modeMenu->setText("Play Morse Code");
    this->setFocus();
    startIt();
}

bool MainWindow::event(QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        keyPressEvent(static_cast<QKeyEvent *>(event));
        event->accept();
        return true;
    } else
        return QMainWindow::event(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    qDebug() << "key pressed: " << event->text() << " " << event->text().size() << " " << event->text().length();
    if (event->key() == Qt::Key_Enter) {
        m_morse->enterPressed();
    }
    if (event->text().size() != 1 || event->text().length() != 1) {
        QMainWindow::keyPressEvent(event);
        return;
    }
    m_morse->keyPressed(event->text().at(event->text().length()-1).toLower());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::startIt()
{

    m_morse->clearList();
    m_morse->add("ws6z");

    m_morse->playSequence();
}

void
MainWindow::createAudioOutput()
{
   QAudioFormat settings;

    settings.setFrequency(44100);
    settings.setChannels(1);
    settings.setSampleSize(16);
    settings.setCodec("audio/pcm");
    settings.setByteOrder(QAudioFormat::LittleEndian);
    settings.setSampleType(QAudioFormat::SignedInt);

    m_audioOutput = new QAudioOutput(settings);

    m_morse = new Morse(this, m_audioOutput, ui);
    connect(m_signalMapper, SIGNAL(mapped(int)), m_morse, SLOT(switchMode(int)));
}

