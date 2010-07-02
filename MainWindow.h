#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia/QAudioOutput>

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

private:
    Ui::MainWindow *ui;
    QAudioOutput   *m_audioOutput;
};

#endif // MAINWINDOW_H
