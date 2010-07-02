#ifndef MORSE_H
#define MORSE_H

#include <QtCore/QString>
#include <QtMultimedia/QAudioOutput>

class Morse
{
public:
    Morse();
    Morse(const QString &sequence, QAudioOutput *output);

    void setSequence(const QString &sequence);
    void playSequence();

private:
    QString m_sequence;
    QAudioOutput *m_output;
};

#endif // MORSE_H
