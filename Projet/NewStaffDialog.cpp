#include "NewStaffDialog.h"
#include "ui_NewStaffDialog.h"

extern DBConnect * db;

NewStaffDialog::NewStaffDialog(QWidget * parent) : QDialog (parent), ui(new Ui::NewStaffDialog)
{
    qDebug() << "Opening new staff dialog";
	ui->setupUi(this);
	QList<ResourceType *> * resources = db->getTypes();
	for(ResourceType * r : *resources)
	{
		ui->typeComboBox->addItem(r->getName(), QVariant::fromValue(static_cast<void *>(r)));
	}
	ui->firstNameLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^[A-Za-zéè '-]+$"), this));
	ui->lastNameLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^[A-Za-zéè '-]+$"), this));
}

NewStaffDialog::~NewStaffDialog()
{
    qDebug() << "Destroying new staff dialog";
	delete ui;
}

Staff * NewStaffDialog::getStaff()
{
	ResourceType * type = static_cast<ResourceType *>(ui->typeComboBox->currentData().value<void *>());

	if(type->getName() == "Informaticien")
		return new Staff(ui->lastNameLineEdit->text(), ui->firstNameLineEdit->text(), type->getId(), type->getName(), ui->loginLineEdit->text(), ui->passwordLineEdit->text());
	else
		return new Staff(ui->lastNameLineEdit->text(), ui->firstNameLineEdit->text(), type->getId(), type->getName());
}

void NewStaffDialog::upperCase_textEdited(const QString &arg1)
{
	const QString &s = arg1;
	QString cap = s.left(1).toUpper();
	QString text = s.length() > 1 ? s.right(s.length() - 1).toLower() : "";
	qobject_cast<QLineEdit *>(sender())->setText(cap + text);
}

void NewStaffDialog::on_firstNameLineEdit_textEdited(const QString &arg1)
{
	upperCase_textEdited(arg1);
}

void NewStaffDialog::on_lastNameLineEdit_textEdited(const QString &arg1)
{
	upperCase_textEdited(arg1);
}

void NewStaffDialog::on_typeComboBox_currentIndexChanged(const QString &arg1)
{
	if(arg1 == "Informaticien")
	{ //Enable login & password
		ui->loginLineEdit->setDisabled(false);
		ui->passwordLineEdit->setDisabled(false);
		ui->loginLineEdit->setStyleSheet("background-color:white;");
		ui->passwordLineEdit->setStyleSheet("background-color:white;");
	}
	else
	{ //Disable login & password
		ui->loginLineEdit->setText("");
		ui->passwordLineEdit->setText("");
		ui->loginLineEdit->setDisabled(true);
		ui->passwordLineEdit->setDisabled(true);
		ui->loginLineEdit->setStyleSheet("background-color:whitesmoke;");
		ui->passwordLineEdit->setStyleSheet("background-color:whitesmoke;");
	}
}

void NewStaffDialog::on_okButton_clicked()
{
	bool valid = true; //Verify fields
	if(ui->lastNameLineEdit->text().isEmpty())
	{
		valid = false;
		ui->lastNameLineEdit->setStyleSheet("background-color:red;");
	}
	else
	{
		ui->lastNameLineEdit->setStyleSheet("background-color:white;");
	}

	if(ui->firstNameLineEdit->text().isEmpty())
	{
		valid = false;
		ui->firstNameLineEdit->setStyleSheet("background-color:red;");
	}
	else
	{
		ui->firstNameLineEdit->setStyleSheet("background-color:white;");
	}

	if(ui->typeComboBox->currentText() == "Informaticien")
	{
		if(ui->loginLineEdit->text().isEmpty())
		{
			valid = false;
			ui->loginLineEdit->setStyleSheet("background-color:red;");
		}
		else
		{
			ui->loginLineEdit->setStyleSheet("background-color:white;");
		}

		if(ui->passwordLineEdit->text().isEmpty())
		{
			valid = false;
			ui->passwordLineEdit->setStyleSheet("background-color:red;");
		}
		else
		{
			ui->passwordLineEdit->setStyleSheet("background-color:white;");
		}
	}

	if(valid) //If all was ok, we valid the closing
		accept();
}

void NewStaffDialog::on_cancelButton_clicked()
{
	reject();
}
