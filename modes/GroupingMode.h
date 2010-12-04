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
    void setSequenceText();

public slots:
    void startNextGroup();
    virtual bool enterPressed();
    virtual void play();
    virtual void clear();

private:
    int     m_groupLength;
    int     m_goodGuesses;
    int     m_badGuesses;
    QString m_currentSequence;
    QString m_enteredWord;
    bool    m_wordWasGood;
};

#endif // GROUPINGMODE_H
