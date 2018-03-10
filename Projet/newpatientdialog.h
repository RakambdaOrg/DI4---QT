#ifndef NEWPATIENTDIALOG_H
#define NEWPATIENTDIALOG_H

#include <QDialog>
#include <QList>

#include "patient.h"
#include "staff.h"
#include "staffitem.h"
#include "dbconnect.h"
#include "ressourcetype.h"
#include "ressourceitem.h"

using namespace std;

namespace Ui {
class NewPatientDialog;
}

class NewPatientDialog : public QDialog
{
    Q_OBJECT

public:
    NewPatientDialog(QWidget *parent = 0);
    ~NewPatientDialog();
    QList<RessourceType *> * getResources();
    Patient * getPatient();

private slots:
    void upperCase_textEdited(const QString &arg1);

    void on_lastNameLineEdit_textEdited(const QString &arg1);
    void on_firstNameLineEdit_textEdited(const QString &arg1);
    void on_cityLineEdit_textEdited(const QString &arg1);

    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::NewPatientDialog *ui;
};

#endif // NEWPATIENTDIALOG_H