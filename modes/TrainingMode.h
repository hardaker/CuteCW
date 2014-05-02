#ifndef TRAININGMODE_H
#define TRAININGMODE_H

#include <QtCore/QSignalMapper>
#include <QtCore/QTime>
#include <QGridLayout>
#include <QAction>
#include <QGridLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QCheckBox>
#include <QProgressBar>

#include "ChooseSequenceDialog.h"

#include "MorseMode.h"
#include "MSequences.h"
#include "GoalBar.h"
#include "ui_MainWindow.h"

class TrainingMode : public MorseMode, MSequences
{
public:
    Q_OBJECT

public:
    TrainingMode(Morse *parent, Ui::MainWindow *ui);
    void setupSequences();
    MorseStat *getStat(const QChar &key);
    void setWPMLabel(float wpm);
    void setupModeWidgets(const QString &sequence, QString barLabel = "");
    virtual void modeMenus();

    virtual void clear();
    virtual void setupTrainingWidgets();
    virtual bool elapsedTimeWasTooLong(int msElapsed, MorseStat *stat);

public slots:

    virtual void handleKeyPress(QChar letterPressed);
    void setSequence(const QString &sequence, int currentlyAt);
    void switchSequence(int sequence);
    void switchSequence(const QString &sequence);
    virtual void audioStopped();
    virtual void setDoEntireSequence(bool value);
    virtual void chooseCustomeSequence();
    virtual void setupWidgets(const QString &sequence, bool includeProgressBars = true, QString barLabel = "");
    virtual QBoxLayout *setupGraphs(QString barLabel = "");
    virtual void updateGraphs();

    void saveStats(QSettings &settings, QString statPrefix = "");
    void loadStats(QSettings &settings, QString statPrefix = "");

private:
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
    bool                            m_acceptForwardKeys;

};



#endif // TRAININGMODE_H
