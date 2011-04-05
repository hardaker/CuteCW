#include "KeyTraining.h"
#include "Morse.h"
#include "MainWindow.h"

#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QTextEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QHBoxLayout>
#include <qdebug.h>

KeyTraining::KeyTraining(Morse *morse, Ui::MainWindow *ui)
    : MorseMode(morse, ui)
{

}

QString KeyTraining::name()
{
    return tr("KeyTraining");
}

QString KeyTraining::helpText()
{
    return tr("this is for practing keying.");
}

void KeyTraining::switchToMode()
{
    // XXX: setup layout
}

void KeyTraining::handleKeyPress(QChar letterPressed)
{
    // XXX: recond the timing
}

void KeyTraining::play()
{
    // pick new keying sequence, and display it
}

void KeyTraining::stop()
{
    // err...  nothing?
}



