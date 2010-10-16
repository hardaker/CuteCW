#ifndef TRAININGMODE_H
#define TRAININGMODE_H

#include "MorseMode.h"
#include "ui_MainWindow.h"

class TrainingMode : public MorseMode
{
public:
    TrainingMode(Morse *parent, Ui::MainWindow *ui);
};

#endif // TRAININGMODE_H
