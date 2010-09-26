#ifndef MORSEMODE_H
#define MORSEMODE_H

#include <QtCore/QObject>
#include <QtCore/QString>

class MorseMode : public QObject
{
public:
    Q_OBJECT

public:
    MorseMode();

public slots:
    virtual void handleKeyPress(QChar letterPressed) = 0;
};

#endif // MORSEMODE_H
