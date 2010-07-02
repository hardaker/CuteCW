#ifndef GENERATOR_H
#define GENERATOR_H

#include <QtCore/QIODevice>

#define FREQ        1200

class Generator : public QIODevice
{
    Q_OBJECT
public:
    Generator(float secs = .5, int freq=1200);
    Generator(Generator *copyFrom);
    ~Generator();

    void appendDataFrom(const Generator *copyFrom);
    void start();
    void stop();

    void clearBuffer();

    char *t;
    int  len;
    int  pos;
    char *buffer;
    bool finished;
    int  chunk_size;
    int  m_freq;
    qint64 bytes_left;

public slots:
    void restartData();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

private:
    int putShort(char *t, unsigned int value);
    int fillData(char *start, int frequency, float seconds);
};



#endif // GENERATOR_H
