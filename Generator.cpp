#include "Generator.h"
#include <qdebug.h>
#include <QtGlobal>
#include <QtCore/QTime>

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SECONDS     1
#define SYSTEM_FREQ 44100

Generator::Generator(float secs, int freq)
    :QIODevice( )
{
    finished = false;
    buffer = new char[int(secs * SYSTEM_FREQ * 4) + 3000];
    t=buffer;
    m_freq = freq;
    len=fillData(t, m_freq, secs); /* mono FREQHz sine */
    pos   = 0;
    bytes_left = len;
}

Generator::Generator(Generator *copyFrom)
    : QIODevice()
{
    buffer = new char[copyFrom->len];
    memcpy(buffer, copyFrom->buffer, copyFrom->len);
    t = buffer;
    m_freq = copyFrom->m_freq;
    len = copyFrom->len;
    pos = 0;
    bytes_left = len;
}

Generator::~Generator()
{
    delete [] buffer;
}

void Generator::clearBuffer() {
    delete buffer;
    buffer = new char[4];
    buffer[0] = buffer[1] = buffer[2] = buffer[3] = 0;
    t = buffer;
    len = bytes_left = 4;
    pos = 0;
}

void Generator::appendDataFrom(const Generator *copyFrom) {
    char *newbuf = new char[len + copyFrom->len];
    memcpy(newbuf, buffer, len);
    memcpy(newbuf + len, copyFrom->buffer, copyFrom->len);
    len += copyFrom->len;
    bytes_left += copyFrom->len;
    delete buffer;
    buffer = t = newbuf;
    // qDebug() << "new left: "<< bytes_left;
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

int Generator::fillData(char *start, int frequency, float seconds)
{
    int i, len=0;
    int value;
    for(i=0; i<int(seconds*SYSTEM_FREQ); i++) {
        if (frequency == 0.0)
            value = 0;
        else
            value=(int)(32767.0*sin(2.0*M_PI*((double)(i))*(double)(frequency)/SYSTEM_FREQ));
        putShort(start, value);
        start += 2;
        len+=2;
    }
    if (frequency > 100) {
        // ramp down or up to 0 for a better drop off in sound
        while(value > 8 || value < -8) {
            i++;
            value=(int)(32767.0*sin(2.0*M_PI*((double)(i))*(double)(frequency)/SYSTEM_FREQ));
            putShort(start, value);
            len += 2;
            start += 2;
        }
    }
    bytes_left = len;
    pos = 0;
    return len;
}

void Generator::restartData()
{
    bytes_left = len;
    pos = 0;
}

qint64 Generator::readData(char *data, qint64 maxlen)
{
    int len = maxlen;
    if (len > 65536)
        len = 65536;

    //qDebug() << "left: " << bytes_left << " / wanted: " << len;

    if (bytes_left == 0) {
        emit generatorDone();
    }

#ifdef Q_OS_LINUX
    // On linux (with Qt 4.7 and 4.7.1) there is a nasty second-long pause/freeze after the audio finishes playing
#define ALWAYS_FILL_WITH_SPACE 1
#else
#define FILL_WITH_SPACE_ONCE 0
#endif

#ifdef ALWAYS_FILL_WITH_SPACE
    if (bytes_left <= 0) {
        // should really only be needed on linux with 4.7 I suspect
        memset(data, 0, maxlen);
        bytes_left = -1;
        return maxlen;
    }
#elif FILL_WITH_SPACE_ONCE
    /* fill with a blank space just once after the starting */
    if (bytes_left < 0) {
        memset(data, 0, maxlen);
        bytes_left = -1;
        return maxlen;
    }
#else
    /* this is how it *should* be done, if the Qt output buffers didn't truncate things */
    if (bytes_left == 0)
        bytes_left = -1;
    if (bytes_left <= 0)
        return -1;
#endif

    if (len < bytes_left) {
        // Normal
        memcpy(data,t+pos,len);
        pos += len;
        bytes_left -= len;
        return len;
    } else {
        // Whats left
        memcpy(data,t+pos,bytes_left);
        bytes_left = 0;
        pos=0;
        return bytes_left;
    }
}

qint64 Generator::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
}

QTime Generator::timeLeft()
{
    int secs = bytes_left/2/SYSTEM_FREQ;
    int msec = ((bytes_left - 2*SYSTEM_FREQ*secs)*1000)/2/SYSTEM_FREQ;
    return QTime(0, 0, secs, msec);
}
