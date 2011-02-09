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
    virtual void play();
    virtual void handleKeyPress(QChar letterPressed);

    virtual QString name();
    virtual QString helpText();

    virtual QTime startNextTrainingKey();
    void startTimerToNextKey(QTime plusTime);
    virtual bool elapsedTimeWasTooLong(int msElapsed, MorseStat *stat);

    virtual void loadSettings(QSettings &settings);
    virtual void saveSettings(QSettings &settings);

public slots:
    void startTimerToNextKey(int plusMSecs = 0);
    void startNextSpeedKey();
    virtual void updateGraphs();
};

#endif // SPEEDTRAININGMODE_H
