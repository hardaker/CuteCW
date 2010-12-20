#ifndef GROUPINGMODE_H
#define GROUPINGMODE_H

#include "TrainingMode.h"
#include "Morse.h"

const int DEFAULT_GROUPLENGTH =1;
const int GROUPLENGTH_WEIGHT = 4;

class GroupingMode : public TrainingMode
{
    Q_OBJECT
public:
    explicit GroupingMode(Morse *parent, Ui::MainWindow *ui);

    virtual void switchToMode();
    virtual void handleKeyPress(QChar letter);

    void createNextSequence();
    virtual void setSequenceText();

    virtual QString helpText();

 signals:
    void groupEntered(int right, int total);

public slots:
    void startNextGroup();
    virtual bool enterPressed();
    virtual void play();
    virtual void clear();

protected:
    int     m_groupLength;
    int     m_goodGuesses;
    int     m_badGuesses;
    QString m_currentSequence;
    QString m_enteredWord;
    bool    m_wordWasGood;
    int     m_rightCount;
};

#endif // GROUPINGMODE_H
