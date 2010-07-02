#ifndef GENERATOR_H
#define GENERATOR_H

#include <QtCore/QIODevice>

#define FREQ        1200

class Generator : public QIODevice
{
    Q_OBJECT
public:
    Generator(float secs = .5, int freq=1200);
    ~Generator();

    void start();
    void stop();

    char *t;
    int  len;
    int  pos;
    int  total;
    char *buffer;
    bool finished;
    int  chunk_size;
    int  m_freq;
    qint64 bytes_left;
    qint64 buf_size;

public slots:
    void restartData();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

private:
    int putShort(char *t, unsigned int value);
    int fillData(char *start, int frequency, float seconds);
};



#endif // GENERATOR_H
