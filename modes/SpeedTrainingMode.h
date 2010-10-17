#ifndef SPEEDTRAININGMODE_H
#define SPEEDTRAININGMODE_H

#include "TrainingMode.h"

class SpeedTrainingMode : public TrainingMode
{
public:
    SpeedTrainingMode(Morse *parent, Ui::MainWindow *ui);

    virtual void switchToMode();
    virtual void handleKeyPress(QChar letterPressed);
};

#endif // SPEEDTRAININGMODE_H
