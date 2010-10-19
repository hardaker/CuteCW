#ifndef TRAININGMODE_H
#define TRAININGMODE_H

#include <QtCore/QSignalMapper>
#include <QtCore/QTime>

#include "MorseMode.h"
#include "ui_MainWindow.h"

class TrainingMode : public MorseMode
{
public:
    enum sequences { KOCH = 0, KOCH1 = 1, KOCH2 = 2, KOCH3 = 3, KOCH4 = 4, ALPHABET = 5 };

    TrainingMode(Morse *parent, Ui::MainWindow *ui);
    void setupSequences();
    void startNextTrainingKey();
    void setDoEntireSequence(bool value);

    virtual void clear();

public slots:
    virtual void play();

    virtual void handleKeyPress(QChar letterPressed);
    void setSequence(const QString &sequence, int currentlyAt);
    void switchSequence(int sequence);

private:
    QString                         m_trainingSequence;
    QStringList                     m_sequences;
    QSignalMapper                  *m_sequenceSignalMapper;
    bool                            m_doEntireSequence;

protected:
    QChar                           m_lastKey;
    QList<QChar>                    m_lastKeys;
    QList<QTime>                    m_lastTimes;
};

#endif // TRAININGMODE_H
