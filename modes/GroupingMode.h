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
    virtual void handleKeyPress(QChar letter);

    void createNextSequence();

public slots:
    void startNextGroup();
    virtual bool enterPressed();
    virtual void play();

private:
    int     m_groupLength;
    QString m_currentSequence;
    QString m_enteredWord;
    bool    m_wordWasGood;
};

#endif // GROUPINGMODE_H
