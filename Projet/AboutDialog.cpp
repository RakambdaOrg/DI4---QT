#include "AboutDialog.h"
#include "ui_aproposdialog.h"

AboutDialog::AboutDialog(QWidget * parent) : QDialog(parent), ui(new Ui::AboutDialog)
{
	ui->setupUi(this);
	this->setWindowTitle("About");
}

AboutDialog::~AboutDialog()
{
	delete ui;
}
