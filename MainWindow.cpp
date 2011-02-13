#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ui_Prefs.h"
#include "ui_AboutDialog.h"
#include <QtMultimedia/QAudioFormat>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QIcon>
#include <qdebug.h>

// for SMALL_DEVICE definition
#include "modes/MorseMode.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_audioOutput(0),
    m_morse(0)

{
    ui->setupUi(this);
#ifdef SMALL_DEVICE
    resize(800,440);
#else
    resize(1000,600);
#endif

    setWindowIcon(QIcon(":/icons/64x64/cutecw.png"));

    createAudioOutput();

    m_morse = new Morse(this, m_audioOutput, ui);

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

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::startIt()
{

    m_morse->clearList();
    //m_morse->add("ws6z");

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
}

void MainWindow::closeEvent(QCloseEvent *event)
 {
    m_morse->saveSettings();
 }
