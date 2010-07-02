#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QtMultimedia/QAudioFormat>
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_audioOutput(0),
    m_gen(0)

{
    ui->setupUi(this);
    connect(ui->start, SIGNAL(clicked()), this, SLOT(startIt()));
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
        m_gen->restartData();
        m_audioOutput->start(m_gen);
    }

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

    m_gen = new Generator(this);
    qDebug() << "here";
    m_gen->start();
    m_audioOutput->start(m_gen);
}

