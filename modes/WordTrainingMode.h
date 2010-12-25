#ifndef WORDTRAININGMODE_H
#define WORDTRAININGMODE_H

#include <QtCore/QSignalMapper>

#include "MorseMode.h"
#include "Morse.h"

class WordTrainingMode : public MorseMode
{
public:
    Q_OBJECT

public:
    enum wordNums { N100 = 0, N200 = 1, N300 = 2, N400 = 3, N500 = 4 };

    WordTrainingMode(Morse *parent, Ui::MainWindow *ui);
    void setupWords();
    void setupWordsMenu();

    virtual void switchToMode();
    virtual bool enterPressed();

    void handleWordResponse(QChar letter);

    virtual QString helpText();

signals:
   void groupEntered(int right, int total);

public slots:
    virtual void handleKeyPress(QChar letterPressed);
    void switchWords(int sequence);

    virtual void play();

private:
    QMap<wordNums, QList<QString> *> words;
    bool                             m_wordWasGood;
    QSignalMapper                   *m_wordSignalMapper;
    wordNums                         m_wordsNumber;
    int                              m_maxWord;
    int                              m_wordnumber;
    QString                          m_enteredWord;
    int                              m_rightCount;
};

#endif // WORDTRAININGMODE_H
