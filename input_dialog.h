#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include "employee.h"

class InputDialog : public QDialog {
    Q_OBJECT
public:
    explicit InputDialog(QWidget *parent = nullptr);

    void setEmployee(const Employee& emp);
    Employee getEmployee() const;

private:
    QLineEdit *editSurname;
    QLineEdit *editName;
    QLineEdit *editPatronymic;
    QLineEdit *editPosition;
    QLineEdit *editDepartment;
    QLineEdit *editSalary;
    QDialogButtonBox *buttonBox;
};
