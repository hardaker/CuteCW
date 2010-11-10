#ifndef TRAININGMODE_H
#define TRAININGMODE_H

#include <QtCore/QSignalMapper>
#include <QtCore/QTime>
#include <QtGui/QGridLayout>

#include "MorseMode.h"
#include "ui_MainWindow.h"

class TrainingMode : public MorseMode
{
public:
    Q_OBJECT

public:
    enum sequences { KOCH = 0, KOCH1 = 1, KOCH2 = 2, KOCH3 = 3, KOCH4 = 4, ALPHABET = 5 };

    TrainingMode(Morse *parent, Ui::MainWindow *ui);
    void setupSequences();
    void setupSequenceButtons(const QString &sequence);
    void startNextTrainingKey();
    void setDoEntireSequence(bool value);
    MorseStat *getStat(const QChar &key);

    virtual void clear();

public slots:
    virtual void play();

    virtual void handleKeyPress(QChar letterPressed);
    virtual void handleKeyPress(const QString &letterPressed);
    void setSequence(const QString &sequence, int currentlyAt);
    void switchSequence(int sequence);
    virtual void audioStopped();

private:
    QStringList                     m_sequences;
    QSignalMapper                  *m_sequenceSignalMapper;
    bool                            m_doEntireSequence;

protected:
    QString                         m_trainingSequence;
    QChar                           m_lastKey;
    QList<QChar>                    m_lastKeys;
    QList<QTime>                    m_lastTimes;
    QMap<QChar, MorseStat *>        m_stats;
    QSignalMapper                  *m_mapper;
    QGridLayout                    *m_buttons;
};

#endif // TRAININGMODE_H
