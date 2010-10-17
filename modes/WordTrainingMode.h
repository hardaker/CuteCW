#ifndef WORDTRAININGMODE_H
#define WORDTRAININGMODE_H

#include <QtCore/QSignalMapper>

#include "MorseMode.h"
#include "Morse.h"

class WordTrainingMode : public MorseMode
{
public:
    enum wordNums { N100 = 0, N200 = 1, N300 = 2, N400 = 3, N500 = 4 };

    WordTrainingMode(Morse *parent, Ui::MainWindow *ui);
    void setupWords();

    virtual void switchToMode();
    virtual bool enterPressed();

    void switchWords(int sequence);
    void handleWordResponse(QChar letter);

public slots:
    virtual void handleKeyPress(QChar letterPressed) = 0;

private:
    QMap<wordNums, QList<QString> *>     words;
    bool                            m_wordWasGood;
    QSignalMapper                   *m_wordSignalMapper;
    wordNums                        m_wordsNumber;
    int                             m_maxWord;
    int                              m_wordnumber;
    QString                          m_enteredWord;
};

#endif // WORDTRAININGMODE_H
