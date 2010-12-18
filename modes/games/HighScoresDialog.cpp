#include "HighScores.h"

#include <QtCore/QSettings>

HighScoresDialog::HighScoresDialog(const QString &tableName, QLinkedList<QPair<int,QString>>  *scores, int newSlot);
  : m_tableName(tableName), m_scores(scores), m_newLot(newSlot)
{

    QVBoxLayout  *layout = new QVBoxLayout();

    scoreTable = new QTextEdit();
    scoreTable->setReadOnly(true);
    scoreTable->append(helpText());
    setScoreTable();
    layout->add(scoreTable);

    if (newSlot != -1) {
        nameEntry = new QLineEdit();
        layout->add(nameEntry);
        connect(nameEntry, SIGNAL(textChanged(QString &)), this, SLOT(textChanged(QString &)));
    }

    QPushButton *ok = new QPushButton(tr("Ok"));
    layout->add(ok);
    connect(ok, SIGNAL(clicked()), this, SLOT(close()));

    setLayout(layout);
}

void HighScoresDialog::setScoreTable() {
    QString text;

    end = m_scores.end();
    for(begin = m_scores.begin(),
          begin != end,
          begin++) {
        text = text + QString("%1\t%2").arg((*begin)->second).arg((*begin)->first);
    }

    scoreTable->setText(text);
}

void HighScoreDialog::textChanged(QString &text) {
    if (m_newSlot == -1)
        return;

    (*m_scores->[m_newSlot]).second = text;
    setScoreTable();
}
