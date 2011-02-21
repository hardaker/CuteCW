#ifndef READMODE_H
#define READMODE_H

#include <QtGui/QTextCursor>
#include <QtGui/QTextEdit>

#include "MorseMode.h"

class ReadMode : public MorseMode
{
public:
    Q_OBJECT

public:
    ReadMode(Morse *parent, Ui::MainWindow *ui);

    void addButtons();

    virtual QString name();
    virtual QString helpText();

public slots:
    virtual void switchToMode();
    virtual void play();
    virtual void stop();
    virtual void readNextLetter();
    virtual void audioStopped();
    virtual void readWordUnderCursor();
    void openFile();

private:
    QTextCursor                     m_readSpot;
    QTextEdit                      *m_textEdit;

};

#endif // READMODE_H
