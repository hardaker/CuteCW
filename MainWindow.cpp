#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ui_AboutDialog.h"
#include <QtMultimedia/QAudioFormat>
#include <QMenu>
#include <QMenuBar>
#include <QIcon>
#include <qdebug.h>

// for SMALL_DEVICE definition
#include "modes/MorseMode.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_audioOutput(0),
    m_morse(0),
    m_devices(new QMediaDevices(this)) {
    ui->setupUi(this);

    m_device = QAudioDevice(m_devices->defaultAudioOutput());

    /*
#ifdef SMALL_DEVICE
    resize(800,440);
#else
    resize(1000,600);
#endif
*/

    //setWindowIcon(QIcon(":/icons/64x64/cutecw.png"));

    createAudioOutput();

    m_morse = new Morse(this, m_audioOutput, ui);

    //this->setFocus();

    startIt();
}

bool MainWindow::event(QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        keyPressEvent(static_cast<QKeyEvent *>(event));
        event->accept();
        return true;
    } else if (event->type() == QEvent::KeyRelease) {
        keyReleaseEvent(static_cast<QKeyEvent *>(event));
        event->accept();
        return true;
    } else
        return QMainWindow::event(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat())  // We don't currently want repeats for any mode
        return;
    if (event->key() == Qt::Key_Enter) {
        qDebug() << " was enter";
        m_morse->enterPressed();
        return;
    }
    if (event->text().size() != 1 || event->text().length() != 1) {
        QMainWindow::keyPressEvent(event);
        return;
    }
    m_morse->keyPressed(event->text().at(event->text().length()-1).toLower());
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat())  // We don't currently want repeats for any mode
        return;
    if (event->text().size() != 1 || event->text().length() != 1) {
        QMainWindow::keyReleaseEvent(event);
        return;
    }
    m_morse->keyReleased(event->text().at(event->text().length()-1).toLower());
}

MainWindow::~MainWindow()
{
    m_morse->saveSettings();
    delete ui;
}

void
MainWindow::startIt()
{

    m_morse->clearList();
    m_morse->add("de ws6z");

    m_morse->playSequence();
}

QAudioSink *
MainWindow::createAudioOutput()
{

//    settings.setFrequency(44100);
   /*
    settings.setSampleSize(16);
    settings.setCodec("audio/pcm");
    settings.setByteOrder(QAudioFormat::LittleEndian);
    settings.setSampleType(QAudioFormat::SignedInt);
    */

    QAudioDevice defaultDeviceInfo(m_devices->defaultAudioOutput());
    QAudioFormat settings = defaultDeviceInfo.preferredFormat();

    settings.setSampleRate(44100);
    settings.setChannelCount(1);
    settings.setSampleFormat(QAudioFormat::Int32);

    qDebug() << "valid audio config:" << settings.isValid();

    qDebug() << "opened: " << defaultDeviceInfo.description();
    
    m_audioOutput = new QAudioSink(defaultDeviceInfo, settings, this);
    m_audioOutput = new QAudioSink(settings, this);
   //m_audioOutput->reset(new QAudioSink(defaultDeviceInfo, settings));
   return m_audioOutput;
}

