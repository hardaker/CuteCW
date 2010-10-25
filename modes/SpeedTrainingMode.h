#ifndef SPEEDTRAININGMODE_H
#define SPEEDTRAININGMODE_H

#include "TrainingMode.h"
#include "Morse.h"

class SpeedTrainingMode : public TrainingMode
{
public:
    Q_OBJECT

public:
    SpeedTrainingMode(Morse *parent, Ui::MainWindow *ui);

    virtual void switchToMode();
    // virtual void handleKeyPress(QChar letterPressed);

public slots:
    void startTimerToNextKey();
    virtual void audioFinished(QAudio::State state);
};

#endif // SPEEDTRAININGMODE_H
