#include "modes/ChooseSequenceDialog.h"

CustomSequenceDialog::CustomSequenceDialog(QString &sequenceSet, QWidget *parent)
    : QDialog(parent)
{
    topLayout = new QVBoxLayout(this);

    grid = new QGridLayout(this);
    topLayout->addLayout(grid);

    mapper = new QSignalMapper(this);

    int row = 0, column = 0;
    const int maxColumn = 5;
    foreach(QChar item, sequenceSet) {
        QCheckBox *box = new QCheckBox(QString(item), this);
        grid->addWidget(box, row, column++);

        connect(box, SIGNAL(toggled(bool)), mapper, SLOT(map()));
        mapper->setMapping(box, QString(item));

        if (column == maxColumn) {
            row++;
            column = 0;
        }
    }

    connect(mapper, SIGNAL(mapped(QString)), this, SLOT(toggle(QString)));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                     QDialogButtonBox::Cancel,
                                     Qt::Horizontal, this);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    topLayout->addWidget(buttonBox);

    setLayout(topLayout);
}

void CustomSequenceDialog::toggle(const QString &item) {
    if (results.contains(item))
        results.remove(item);
    else
        results.insert(item);
}

QString CustomSequenceDialog::getResults() {
    QString results;
    foreach(QString item, results) {
        results += item;
    }
    return results;
}


