#ifndef WORDTRAININGMODE_H
#define WORDTRAININGMODE_H

#include <QtCore/QSignalMapper>
#include <QtWidgets/QSpinBox>

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
    virtual void setSequenceText();

    void handleWordResponse(QChar letter);

    virtual QString name();
    virtual QString helpText();
    virtual QString icon();

    virtual void loadSettings(QSettings &settings);
    virtual void saveSettings(QSettings &settings);

    virtual QBoxLayout *getPrefsLayout();
    void acceptPrefs();

signals:
   void groupEntered(int right, int total);

public slots:
    virtual void handleKeyPress(QChar letterPressed);
    void switchWords(int sequence);

    virtual void play();

protected:
    int                              m_maxWord;
    QMap<wordNums, QList<QString> *> words;
    wordNums                         m_wordsNumber;

private:
    bool                             m_wordWasGood;
    QSignalMapper                   *m_wordSignalMapper;
    int                              m_wordnumber;
    QString                          m_enteredWord;
    int                              m_rightCount;
    QSpinBox                        *m_letterSpacingBox;
    int                              m_letterSpacing;
};

#endif // WORDTRAININGMODE_H
