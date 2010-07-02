#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia/QAudioOutput>
#include "Generator.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createAudioOutput();

public slots:
    void startIt();

private:
    Ui::MainWindow *ui;
    QAudioOutput   *m_audioOutput;
    Generator      *m_gen;
};

#endif // MAINWINDOW_H
