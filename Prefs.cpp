#include "Prefs.h"
#include "Morse.h"

#include <QtGui/QTabWidget>
#include <QtGui/QFormLayout>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>

Prefs::Prefs(Morse *morse, QWidget *parent) :
    QDialog(parent), m_morse(morse)
{
    QVBoxLayout *topLayout = new QVBoxLayout();
    QTabWidget *tabWidget = new QTabWidget();
    QWidget *genericPrefs = new QWidget();
    QFormLayout *genericForm = new QFormLayout();

    m_WPMRate = new QSpinBox();
    m_WPMRate->setRange(1,40);
    m_WPMRate->setValue(m_morse->currentWPMGoal());
    genericForm->addRow(tr("WPM Play Rate"), m_WPMRate);

    m_acceptRate = new QSpinBox();
    m_WPMRate->setRange(1,40);
    m_WPMRate->setValue(m_morse->currentWPMAccept());
    genericForm->addRow(tr("Acceptance Rate"), m_WPMRate);

    m_tone = new QSpinBox();
    m_tone->setRange(100,1200);
    m_tone->setValue(m_morse->tone());
    genericForm->addRow(tr("Tone Frequency"), m_tone);

    tabWidget->addTab(genericPrefs, tr("General"));

    topLayout->addWidget(tabWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                       | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(cancel()));
    topLayout->addWidget(buttonBox);

    setLayout(topLayout);
}

void Prefs::ok()
{
    m_morse->setTone(m_tone->value());
    m_morse->setWPMGoal(m_WPMRate->value());
    m_morse->setWPMAccept(m_acceptRate->value());
    m_morse->saveSettings();
    m_morse->loadSettings();
}

void Prefs::cancel()
{
}

Prefs::~Prefs()
{
}
