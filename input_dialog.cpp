#include "input_dialog.h"
#include <QVBoxLayout>

InputDialog::InputDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Редактор сотрудника");

    auto *layout = new QVBoxLayout(this);
    auto *formLayout = new QFormLayout();

    editSurname = new QLineEdit();
    editName = new QLineEdit();
    editPatronymic = new QLineEdit();
    editPosition = new QLineEdit();
    editDepartment = new QLineEdit();
    editSalary = new QLineEdit();

    formLayout->addRow("Фамилия:", editSurname);
    formLayout->addRow("Имя:", editName);
    formLayout->addRow("Отчество:", editPatronymic);
    formLayout->addRow("Должность:", editPosition);
    formLayout->addRow("Отдел:", editDepartment);
    formLayout->addRow("Зарплата:", editSalary);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);
}

void InputDialog::setEmployee(const Employee& emp) {
    editSurname->setText(emp.getSurname());
    editName->setText(emp.getName());
    editPatronymic->setText(emp.getPatronymic());
    editPosition->setText(emp.getPosition());
    editDepartment->setText(emp.getDepartment());
    editSalary->setText(emp.getSalary());
}

Employee InputDialog::getEmployee() const {
    return Employee(editSurname->text(), editName->text(), editPatronymic->text(),
                    editPosition->text(), editDepartment->text(), editSalary->text());
}
