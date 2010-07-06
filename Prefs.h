#ifndef PREFS_H
#define PREFS_H

#include <QDialog>

namespace Ui {
    class Prefs;
}

class Prefs : public QDialog
{
    Q_OBJECT

public:
    explicit Prefs(QWidget *parent = 0);
    ~Prefs();

private:
    Ui::Prefs *ui;
};

#endif // PREFS_H
