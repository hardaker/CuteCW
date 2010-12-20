#include "HighScoresDialog.h"

#include <QtCore/QSettings>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <qdebug.h>

HighScoresDialog::HighScoresDialog(const QString &tableName, QList<QPair<int,QString> >  *scores, int newSlot)
  : m_tableName(tableName), m_scores(scores), m_newSlot(newSlot)
{
    qDebug() << "new slot: " << newSlot;
    QVBoxLayout  *layout = new QVBoxLayout();

    scoreTable = new QTextEdit();
    scoreTable->setReadOnly(true);
    setScoreTable();
    layout->addWidget(scoreTable);

    QHBoxLayout *hbox = new QHBoxLayout();

    if (newSlot != -1) {
        QLabel *scoreName = new QLabel(tr("Your Name: "));
        hbox->addWidget(scoreName);

        nameEntry = new QLineEdit();
        hbox->addWidget(nameEntry);
        connect(nameEntry, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
    } else {
        QLabel *scoreName = new QLabel(tr("Your Score: "));
        hbox->addWidget(scoreName);
    }

    QLabel *score = new QLabel(QString::number(m_scores->at(newSlot).first));
    hbox->addWidget(score);

    layout->addLayout(hbox);

    QPushButton *ok = new QPushButton(tr("Ok"));
    layout->addWidget(ok);
    connect(ok, SIGNAL(clicked()), this, SLOT(close()));

    setLayout(layout);
}

void HighScoresDialog::setScoreTable() {
    QString text = "<table>";
    int count = 0;

    QList<QPair<int,QString> >::iterator end = m_scores->end();
    for(QList<QPair<int,QString> >::iterator begin = m_scores->begin();
        begin != end;
        begin++) {
        if (m_newSlot == count)
            text = text + QString("<tr><td><font color=\"green\">%1</font></td><td><font color=\"green\">%2</font></td></tr>").arg((*begin).second).arg((*begin).first);
        else
            text = text + QString("<tr><td>%1</td><td>%2</td></tr>").arg((*begin).second).arg((*begin).first);
        count++;
    }
    text = text + "</table>";

    scoreTable->setText(text);
}

void HighScoresDialog::textChanged(QString text) {
    if (m_newSlot == -1)
        return;

    (*m_scores)[m_newSlot].second = text;
    setScoreTable();
}
