#ifndef GROUPINGMODE_H
#define GROUPINGMODE_H

#include "TrainingMode.h"
#include "Morse.h"

const int DEFAULT_GROUPLENGTH = 3;

class GroupingMode : public TrainingMode
{
    Q_OBJECT
public:
    explicit GroupingMode(Morse *parent, Ui::MainWindow *ui);

    virtual void switchToMode();
    virtual void handleKeyPress(QChar letterPressed);

private:
    int groupLength;
    QString currentSequence;

};

#endif // GROUPINGMODE_H
