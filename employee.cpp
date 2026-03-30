#include "employee.h"

Employee::Employee(const QString &surname, const QString &name, const QString &patronymic,
                   const QString &position, const QString &department, const QString &salary)
    : m_surname(surname), m_name(name), m_patronymic(patronymic),
    m_position(position), m_department(department), m_salary(salary)
{
}

QString Employee::getSurname() const { return m_surname; }
QString Employee::getName() const { return m_name; }
QString Employee::getPatronymic() const { return m_patronymic; }
QString Employee::getPosition() const { return m_position; }
QString Employee::getDepartment() const { return m_department; }
QString Employee::getSalary() const { return m_salary; }

void Employee::setSurname(const QString &value) { m_surname = value; }
void Employee::setName(const QString &value) { m_name = value; }
void Employee::setPatronymic(const QString &value) { m_patronymic = value; }
void Employee::setPosition(const QString &value) { m_position = value; }
void Employee::setDepartment(const QString &value) { m_department = value; }
void Employee::setSalary(const QString &value) { m_salary = value; }

QDataStream &operator<<(QDataStream &out, Employee &emp) {
    out << emp.m_surname << emp.m_name << emp.m_patronymic
        << emp.m_position << emp.m_department << emp.m_salary;
    return out;
}

QDataStream &operator>>(QDataStream &in,  Employee &emp) {
    in >> emp.m_surname >> emp.m_name >> emp.m_patronymic
        >> emp.m_position >> emp.m_department >> emp.m_salary;
    return in;
}
