#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ui_Prefs.h"
#include <QtMultimedia/QAudioFormat>
#include <QtGui/QMenu>
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

    QMenu *modeMenu = new QMenu(ui->modeMenu);
    ui->modeMenu->setMenu(modeMenu);

    QAction *action = modeMenu->addAction("Play Morse Code");
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::PLAY);

    action = modeMenu->addAction("Recognition Train");
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::TRAIN);

    createAudioOutput();
    connect(ui->prefs, SIGNAL(clicked()), m_morse, SLOT(prefsButton()));
    ui->modeMenu->setText("Play Morse Code");
    startIt();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    qDebug() << "key pressed: " << event->text();
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
    m_morse->add('w');
    m_morse->add('s');
    m_morse->add('6');
    m_morse->add('z');

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

