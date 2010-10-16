#ifndef MORSEMODE_H
#define MORSEMODE_H

#include <QtCore/QObject>
#include <QtCore/QString>

// #include "Morse.h"
#include "ui_MainWindow.h"

class Morse;

class MorseMode : public QObject
{
public:
    Q_OBJECT

public:
    MorseMode(Morse *morse);
    Morse *morseParent();

public slots:
    virtual void handleKeyPress(QChar letterPressed) = 0;
    virtual void switchToMode(Ui::MainWindow *m_ui) = 0;

    virtual void playButton();

protected:
    Morse   *m_morse;
};

#endif // MORSEMODE_H
