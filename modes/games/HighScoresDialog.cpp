#include "HighScoresDialog.h"

#include <QtCore/QSettings>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>

HighScoresDialog::HighScoresDialog(const QString &tableName, QList<QPair<int,QString> >  *scores, int newSlot)
  : m_tableName(tableName), m_scores(scores), m_newSlot(newSlot)
{

    QVBoxLayout  *layout = new QVBoxLayout();

    scoreTable = new QTextEdit();
    scoreTable->setReadOnly(true);
    setScoreTable();
    layout->addWidget(scoreTable);

    if (newSlot != -1) {
        nameEntry = new QLineEdit();
        layout->addWidget(nameEntry);
        connect(nameEntry, SIGNAL(textChanged(QString &)), this, SLOT(textChanged(QString &)));
    }

    QPushButton *ok = new QPushButton(tr("Ok"));
    layout->addWidget(ok);
    connect(ok, SIGNAL(clicked()), this, SLOT(close()));

    setLayout(layout);
}

void HighScoresDialog::setScoreTable() {
    QString text;

    QList<QPair<int,QString> >::iterator end = m_scores->end();
    for(QList<QPair<int,QString> >::iterator begin = m_scores->begin();
        begin != end;
        begin++) {
        text = text + QString("%1\t%2\n").arg((*begin).second).arg((*begin).first);
    }

    scoreTable->setText(text);
}

void HighScoresDialog::textChanged(QString &text) {
    if (m_newSlot == -1)
        return;

    m_scores->value(m_newSlot).second = text;
    setScoreTable();
}
