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
    virtual QTime startNextTrainingKey();
    virtual bool elapsedTimeWasTooLong(int msElapsed, MorseStat *stat);

    virtual QString name();
    virtual QString helpText();

    virtual void loadSettings(QSettings &settings);
    virtual void saveSettings(QSettings &settings);

    virtual QBoxLayout * getPrefsLayout();
    virtual void acceptPrefs();
public slots:
    virtual void updateGraphs();

private:
    int m_percentGoal;
    int m_minimumTries;

    QSpinBox *m_percentGoalBox;
    QSpinBox *m_minimumTriesBox;
};

#endif // LETTERTRAININGMODE_H
