#ifndef SPEEDTRAININGMODE_H
#define SPEEDTRAININGMODE_H

#include <QtGui/QSpinBox>

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
    virtual QString icon();

    virtual QTime startNextTrainingKey();
    void startTimerToNextKey(QTime plusTime);
    virtual bool elapsedTimeWasTooLong(int msElapsed, MorseStat *stat);

    virtual void loadSettings(QSettings &settings);
    virtual void saveSettings(QSettings &settings);

    virtual QBoxLayout * getPrefsLayout();
    virtual void acceptPrefs();
public slots:
    void startTimerToNextKey(int plusMSecs = 0);
    void startNextSpeedKey();
    virtual void updateGraphs();

private:
    int                             m_countWeight;
    float                           m_speedMultiplier;

    QSpinBox *m_acceptRateBox;
    QSpinBox *m_countWeightBox;
    QDoubleSpinBox *m_speedMultiplierBox;
};

#endif // SPEEDTRAININGMODE_H
