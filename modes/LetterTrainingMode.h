#ifndef LETTERTRAININGMODE_H
#define LETTERTRAININGMODE_H

#include <QtGui/QSpinBox>

#include "TrainingMode.h"

class LetterTrainingMode : public TrainingMode
{
public:
    Q_OBJECT

public:
    LetterTrainingMode(Morse *parent, Ui::MainWindow *ui);

    virtual void switchToMode();
    virtual void handleKeyPress(QChar letterPressed);
    virtual bool elapsedTimeWasTooLong(int msElapsed, MorseStat *stat);

    virtual QString name();
    virtual QString helpText();
    virtual QString icon();

    virtual void loadSettings(QSettings &settings);
    virtual void saveSettings(QSettings &settings);

    virtual QBoxLayout * getPrefsLayout();
    virtual void acceptPrefs();

public slots:
    virtual void updateGraphs();
    virtual void startNextTrainingKey();
    virtual void play();

private:
    int m_percentGoal;
    int m_minimumTries;
    float m_letterDelay;

    QSpinBox *m_percentGoalBox;
    QSpinBox *m_minimumTriesBox;
    QDoubleSpinBox *m_letterDelayBox;

};

#endif // LETTERTRAININGMODE_H
