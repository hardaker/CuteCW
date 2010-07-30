#ifndef GENERATOR_H
#define GENERATOR_H

#include <QtCore/QIODevice>

#define FREQ        1200
// 1764 = 20wpm dit in length; three of these seems to be a safe buffer length to extend
#define ZEROLENGTH  1764
#define ZEROCOUNTS     3

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
    void restartPauses();
    void setupPauses();

    char *t;
    int  len;
    int  pos;
    char *buffer;
    char zerobuffer[ZEROLENGTH];
    bool finished;
    int  chunk_size;
    int  m_freq;
    int  m_zerocount;
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
