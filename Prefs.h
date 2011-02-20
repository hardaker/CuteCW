#ifndef PREFS_H
#define PREFS_H
#include "Morse.h"
#include <QDialog>

namespace Ui {
    class Prefs;
}

class Prefs : public QDialog
{
    Q_OBJECT

public:
    explicit Prefs(Morse *morse, QWidget *parent = 0);
    ~Prefs();

public slots:
    void ok();
    void cancel();
private:
    Morse *m_morse;

    QSpinBox *m_WPMRate;
    QSpinBox *m_tone;

};

#endif // PREFS_H
