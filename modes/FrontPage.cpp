#include "FrontPage.h"
#include "Morse.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QtCore/QSignalMapper>

FrontPage::FrontPage(Morse *parent, Ui::MainWindow *ui)
    : MorseMode(parent, ui)
{
}

void FrontPage::switchToMode()
{
    m_ui->helpBar->setText("<font color=\"green\">Selecting a starting place from the buttons below</font>");

    m_ui->letter->hide();
    m_ui->play->hide();
    m_ui->changeSequence->hide();

    m_ui->homeButton->hide();

    setupWidgets();
}

void FrontPage::setupWidgets() {
    QSignalMapper *mapper = new QSignalMapper(m_ui->forModes);
    QVBoxLayout *topVBox = new QVBoxLayout();
    QHBoxLayout *mainHBox = new QHBoxLayout();
    topVBox->setStretchFactor(mainHBox, 5);
    topVBox->addLayout(mainHBox);

    QGroupBox *leftGroup = new QGroupBox(tr("Training Modes"));
    QGroupBox *rightGroup = new QGroupBox(tr("Play Modes"));
#ifndef SMALL_DEVICE
    QFont groupFont = leftGroup->font();
    groupFont.setPointSize(3 * groupFont.pointSize() / 2);
    leftGroup->setFont(groupFont);
    rightGroup->setFont(groupFont);
#endif

    QVBoxLayout *leftBox = new QVBoxLayout();
    leftGroup->setLayout(leftBox);
    mainHBox->addWidget(leftGroup);
    QVBoxLayout *rightBox = new QVBoxLayout();
    rightGroup->setLayout(rightBox);
    mainHBox->addWidget(rightGroup);

    QPushButton *button;
    QList<Morse::TrainingMode> leftContents, rightContents;
    leftContents << Morse::TRAIN << Morse::SPEEDTRAIN << Morse::WORDS << Morse::GROUPS << Morse::KEYING;
    rightContents << Morse::PLAY << Morse::READ << Morse::WORDGAME << Morse::GROUPGAME << Morse::KEYINVADERS;

    foreach(Morse::TrainingMode themode, leftContents) {
        button = new QPushButton(QIcon(m_morse->getMode(themode)->icon()), m_morse->getMode(themode)->name());
        button->setIconSize(QSize(64,64));
        QSizePolicy policy = button->sizePolicy();
        policy.setVerticalPolicy(QSizePolicy::Expanding);
        button->setSizePolicy(policy);
        leftBox->addWidget(button);
        connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
        mapper->setMapping(button, (int) themode);
    }

    foreach(Morse::TrainingMode themode, rightContents) {
        button = new QPushButton(QIcon(m_morse->getMode(themode)->icon()), m_morse->getMode(themode)->name());
        button->setIconSize(QSize(64,64));
        QSizePolicy policy = button->sizePolicy();
        policy.setVerticalPolicy(QSizePolicy::Expanding);
        button->setSizePolicy(policy);
        rightBox->addWidget(button);
        connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
        mapper->setMapping(button, (int) themode);
    }

    connect(mapper, SIGNAL(mappedInt(int)), m_morse, SLOT(switchMode(int)));

    m_ui->forModes->addLayout(topVBox);
    m_ui->forModes->setStretchFactor(topVBox, 5);
}


QString FrontPage::helpText()
{
    return tr("<p>From this screen you can access all the other modes.  This is just a starting screen.  "
              "<p>If you want to learn morse code, consider following the steps in the left hand panel.  "
              "Then, to continue your training and/or have some fun in the process, try the modes in the right hand panel."
              "<p>once you enter a mode, use the menus to navegate to other modes.</p>");
}

QString FrontPage::name()
{
    return tr("Home Page");
}
