#ifndef READMODE_H
#define READMODE_H

#include <QtGui/QTextCursor>
#include <QtGui/QTextEdit>
#include <QtGui/QSpinBox>

#include "MorseMode.h"

class ReadMode : public MorseMode
{
public:
    Q_OBJECT

public:
    ReadMode(Morse *parent, Ui::MainWindow *ui);

    void addButtons();

    virtual QString name();
    virtual QString helpText();
    virtual QString icon();

    virtual void loadSettings(QSettings &settings);
    virtual void saveSettings(QSettings &settings);
    virtual QBoxLayout * getPrefsLayout();
    virtual void acceptPrefs();

public slots:
    virtual void switchToMode();
    virtual void play();
    virtual void stop();
    virtual void audioStopped();
    virtual void readWordUnderCursor();
    void openFile();

private:
    QTextCursor                     m_readSpot;
    QTextEdit                      *m_textEdit;
    QString                         m_currentText;
    int                             m_readWordCount;
    int                             m_currentWordCount;
    QSpinBox                       *m_readWordCountBox;
};

#endif // READMODE_H
