#ifndef LETTERTRAININGMODE_H
#define LETTERTRAININGMODE_H

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

    virtual QString name();
    virtual QString helpText();

private:
    static const int m_percentGoal = 90;
    static const int m_minimumTries = 5;
};

#endif // LETTERTRAININGMODE_H
