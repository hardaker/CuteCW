#include "QModeStart.h"

#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QModeStart::QModeStart(QWidget *parent, QString modeTitle) :
    QDialog(parent)
{
    QVBoxLayout  *layout = new QVBoxLayout();

    if (modeTitle.length() > 0) {
        QLabel *title = new QLabel(tr("<b>%1</b>").arg(modeTitle));
        layout->addWidget(title);
    }

    // user specific middle
    m_main = new QVBoxLayout();
    layout->addLayout(m_main);

    QHBoxLayout *hbox = new QHBoxLayout();
    layout->addLayout(hbox);

    QPushButton *ok = new QPushButton(tr("Ok"));
    hbox->addWidget(ok);
    connect(ok, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *noPlay = new QPushButton(tr("Don't Play"));
    hbox->addWidget(noPlay);
    connect(noPlay, SIGNAL(clicked()), this, SLOT(reject()));

    setLayout(layout);
}

QVBoxLayout *QModeStart::mainLayout() {
    return m_main;
}
