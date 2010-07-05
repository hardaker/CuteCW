#include "MainWindow.h"
#include "ui_MainWindow.h"
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
    connect(ui->start, SIGNAL(clicked()), this, SLOT(startIt()));

    // setup mode menu
    m_signalMapper = new QSignalMapper(this);

    QMenu *modeMenu = new QMenu(ui->modeMenu);
    ui->modeMenu->setMenu(modeMenu);

    QAction *action = modeMenu->addAction("play");
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::PLAY);

    action = modeMenu->addAction("train");
    connect(action, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(action, (int) Morse::TRAIN);

    createAudioOutput();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::startIt()
{
    if (! m_audioOutput)
        createAudioOutput();
    else {
        qDebug() << "restarting";
    }
    m_morse->clearList();
    //m_morse->add('w');
    //m_morse->add('s');

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
    qDebug() << "here";

    m_morse = new Morse(m_audioOutput, ui->status, ui->sequence);
    connect(ui->input, SIGNAL(textChanged(QString)), m_morse, SLOT(keyPressed(QString)));
    connect(m_signalMapper, SIGNAL(mapped(int)), m_morse, SLOT(switchMode(int)));
}

