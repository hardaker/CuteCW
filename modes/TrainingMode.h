#ifndef TRAININGMODE_H
#define TRAININGMODE_H

#include <QtCore/QSignalMapper>
#include <QtCore/QTime>
#include <QtGui/QGridLayout>
#include <QtGui/QAction>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QMessageBox>
#include <QtGui/QInputDialog>
#include <QtGui/QCheckBox>

#include "ChooseSequenceDialog.h"

#include "MorseMode.h"
#include "ui_MainWindow.h"

class TrainingMode : public MorseMode
{
public:
    Q_OBJECT

public:
    enum sequences { KOCH = 0, KOCH1 = 1, KOCH2 = 2, KOCH3 = 3, KOCH4 = 4,
                     ALPHABET = 5, AF = 6, GM = 7, NT = 8, UZ = 9,
                     NUMBERS = 10, SYMBOLS = 11, EVERYTHING = 12};

    TrainingMode(Morse *parent, Ui::MainWindow *ui);
    void setupSequences();
    QTime startNextTrainingKey();
    MorseStat *getStat(const QChar &key);
    void setWPMLabel(float wpm);

    virtual void clear();
    virtual void setupTrainingWidgets();

public slots:
    virtual void play();

    virtual void handleKeyPress(QChar letterPressed);
    void setSequence(const QString &sequence, int currentlyAt);
    void switchSequence(int sequence);
    void switchSequence(const QString &sequence);
    virtual void audioStopped();
    virtual void setDoEntireSequence(bool value);
    virtual void chooseCustomeSequence();
    virtual void setupWidgets(const QString &sequence);

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
    QAction                        *m_doEntireSequenceButton;
    int                             m_maxBadLetters;
};



#endif // TRAININGMODE_H
