#pragma once
#include <QString>
#include <QDataStream>

class Employee {
public:
    Employee() = default;

    Employee(const QString &surname, const QString &name, const QString &patronymic,
             const QString &position, const QString &department, const QString &salary);

    QString getSurname() const;
    QString getName() const;
    QString getPatronymic() const;
    QString getPosition() const;
    QString getDepartment() const;
    QString getSalary() const;

    void setSurname(const QString &value);
    void setName(const QString &value);
    void setPatronymic(const QString &value);
    void setPosition(const QString &value);
    void setDepartment(const QString &value);
    void setSalary(const QString &value);

    ~Employee() = default;

    friend QDataStream& operator<<(QDataStream& out, Employee& emp);
    friend QDataStream& operator>>(QDataStream& in, Employee& emp);

private:
    QString m_surname {};
    QString m_name {};
    QString m_patronymic {};
    QString m_position {};
    QString m_department {};
    QString m_salary {};
};
