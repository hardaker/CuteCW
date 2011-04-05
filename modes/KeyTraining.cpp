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

