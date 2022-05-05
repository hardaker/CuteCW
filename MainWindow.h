#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioOutput>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QSignalMapper>
#include <QKeyEvent>
#include <QAudioSink>
#include "Generator.h"
#include "Morse.h"

namespace Ui {
    class MainWindow;
    class Prefs;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QAudioSink *createAudioOutput();
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    bool event(QEvent *event);

public slots:
    void startIt();

private:
    Ui::MainWindow *ui;
    Ui::Prefs      *prefs;
    QAudioSink     *m_audioOutput;
    Morse          *m_morse;
    QSignalMapper  *m_signalMapper;
    QMediaDevices  *m_devices = nullptr;
};

#endif // MAINWINDOW_H
