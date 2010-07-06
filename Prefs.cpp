#include "Prefs.h"
#include "ui_Prefs.h"

Prefs::Prefs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Prefs)
{
    ui->setupUi(this);
}

Prefs::~Prefs()
{
    delete ui;
}
