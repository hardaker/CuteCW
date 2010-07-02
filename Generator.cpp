#include "Generator.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SECONDS     1
#define FREQ        1500
#define SYSTEM_FREQ 44100

Generator::Generator(QObject *parent)
    :QIODevice( parent )
{
    finished = false;
    buffer = new char[SECONDS*SYSTEM_FREQ*4+1000];
    t=buffer;
    len=fillData(t,FREQ,SECONDS); /* mono FREQHz sine */
    pos   = 0;
    total = len;
}

Generator::~Generator()
{
    delete [] buffer;
}

void Generator::start()
{
    open(QIODevice::ReadOnly);
}

void Generator::stop()
{
    close();
}

int Generator::putShort(char *t, unsigned int value)
{
    *(unsigned char *)(t++)=value&255;
    *(unsigned char *)(t)=(value/256)&255;
    return 2;
}

int Generator::fillData(char *start, int frequency, int seconds)
{
    int i, len=0;
    int value;
    for(i=0; i<seconds*SYSTEM_FREQ; i++) {
        value=(int)(32767.0*sin(2.0*M_PI*((double)(i))*(double)(frequency)/SYSTEM_FREQ));
        putShort(start, value);
        start += 4;
        len+=2;
    }
    return len;
}

qint64 Generator::readData(char *data, qint64 maxlen)
{
    int len = maxlen;
    if (len > 16384)
        len = 16384;

    if (len < (SECONDS*SYSTEM_FREQ*2)-pos) {
        // Normal
        memcpy(data,t+pos,len);
        pos+=len;
        return len;
    } else {
        // Whats left and reset to start
        qint64 left = (SECONDS*SYSTEM_FREQ*2)-pos;
        memcpy(data,t+pos,left);
        pos=0;
        return left;
    }
}

qint64 Generator::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
}
