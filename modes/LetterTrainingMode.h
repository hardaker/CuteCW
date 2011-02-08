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
};

#endif // LETTERTRAININGMODE_H
