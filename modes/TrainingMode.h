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
#include <QtGui/QProgressBar>

#include "ChooseSequenceDialog.h"

#include "MorseMode.h"
#include "GoalBar.h"
#include "ui_MainWindow.h"

class TrainingMode : public MorseMode
{
public:
    Q_OBJECT

public:
    enum sequences { KOCH, KOCH1, KOCH2, KOCH3, KOCH4,
                     ALPHABET, AF, GM, NT, UZ,
                     NUMBERS, SYMBOLS,
                     EVERYTHING};

    TrainingMode(Morse *parent, Ui::MainWindow *ui);
    void setupSequences();
    virtual QTime startNextTrainingKey();
    MorseStat *getStat(const QChar &key);
    void setWPMLabel(float wpm);
    void setupModeWidgets(const QString &sequence, QString barLabel = "");

    virtual void clear();
    virtual void setupTrainingWidgets();
    virtual bool elapsedTimeWasTooLong(int msElapsed, MorseStat *stat);

public slots:
    virtual void play();

    virtual void handleKeyPress(QChar letterPressed);
    void setSequence(const QString &sequence, int currentlyAt);
    void switchSequence(int sequence);
    void switchSequence(const QString &sequence);
    virtual void audioStopped();
    virtual void setDoEntireSequence(bool value);
    virtual void chooseCustomeSequence();
    virtual void setupWidgets(const QString &sequence, bool includeProgressBars = true, QString barLabel = "");
    virtual QGridLayout *setupGraphs(QString barLabel = "");
    virtual void updateGraphs();

    void saveStats(QSettings &settings, QString statPrefix = "");
    void loadStats(QSettings &settings, QString statPrefix = "");

private:
    QStringList                     m_sequences;
    QSignalMapper                  *m_sequenceSignalMapper;

protected:
    bool                            m_doEntireSequence;
    QString                         m_trainingSequence;
    QChar                           m_lastKey;
    QList<QChar>                    m_lastKeys;
    QList<QTime>                    m_lastTimes;
    QMap<QChar, GoalBar *>          m_progressBars;
    QMap<QChar, QLabel *>           m_progressLabels;
    QMap<QChar, MorseStat *>        m_stats;
    QAction                        *m_doEntireSequenceButton;
    int                             m_maxBadLetters;
    bool                            m_includeProgressBars;

};



#endif // TRAININGMODE_H
