#include "HighScoresDialog.h"

#include <QtCore/QSettings>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <qdebug.h>

HighScoresDialog::HighScoresDialog(const QString &tableName, QList<QPair<int,QString> >  *scores, int score, int newSlot)
  : m_tableName(tableName), m_scores(scores), m_newSlot(newSlot), nameEntry(0)
{
    QVBoxLayout  *layout = new QVBoxLayout();

    scoreTable = new QTextEdit();
    scoreTable->setReadOnly(true);
    setScoreTable();
    layout->addWidget(scoreTable);

    QHBoxLayout *hbox = new QHBoxLayout();

    if (newSlot != -1) {
        QSettings settings("WS6Z", "qtcw");
        m_savedName = settings.value("highscoresdialog/defaultName").toString();

        QLabel *scoreName = new QLabel(tr("Your Name: "));
        hbox->addWidget(scoreName);

        nameEntry = new QLineEdit();
        nameEntry->setText(m_savedName);
        hbox->addWidget(nameEntry);
        connect(nameEntry, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));

        textChanged(m_savedName);
    } else if (score > -1) {
        QLabel *scoreName = new QLabel(tr("Your Score: "));
        hbox->addWidget(scoreName);
    }

    if (newSlot > -1 && score == -1)
        score = m_scores->at(newSlot).first;

    if (score > -1) {
        QLabel *scoreLabel = new QLabel(QString::number(score));
        hbox->addWidget(scoreLabel);
    }

    layout->addLayout(hbox);

    QPushButton *ok = new QPushButton(tr("Ok"));
    layout->addWidget(ok);
    connect(ok, SIGNAL(clicked()), this, SLOT(close()));
    connect(this, SIGNAL(destroyed()), this, SLOT(saveDefaultName()));

    setLayout(layout);

    if (nameEntry)
        nameEntry->setFocus(Qt::OtherFocusReason);
}

void HighScoresDialog::setScoreTable() {
    QString text = "<table>";
    int count = 0;

    QList<QPair<int,QString> >::iterator end = m_scores->end();
    for(QList<QPair<int,QString> >::iterator begin = m_scores->begin();
        begin != end;
        begin++) {
        if (m_newSlot == count)
            text = text + QString("<tr><td><font color=\"green\"><a anchor=\"newslot\">%1</font></td><td><font color=\"green\">%2</font></td></tr>").arg((*begin).second).arg((*begin).first);
        else
            text = text + QString("<tr><td>%1</td><td>%2</td></tr>").arg((*begin).second).arg((*begin).first);
        count++;
    }
    text = text + "</table>";

    scoreTable->setText(text);
    scoreTable->scrollToAnchor("newslot");
}

void HighScoresDialog::textChanged(QString text) {
    if (m_newSlot == -1)
        return;

    (*m_scores)[m_newSlot].second = text;
    setScoreTable();
    m_savedName = text;
}

void HighScoresDialog::saveDefaultName() {
    QSettings settings("WS6Z", "qtcw");
    settings.setValue("highscoresdialog/defaultName", m_savedName);
}

