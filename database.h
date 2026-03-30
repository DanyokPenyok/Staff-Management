#pragma once
#include <QAbstractTableModel>
#include "employee.h"
#include "vector.h"

class DataBase : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void addEmployee(const Employee &emp);
    void deleteEmployee(int row);
    void editEmployee(int row, const Employee &emp);
    const Employee& getEmployee(int row) const;

    void saveToFile(const QString &filename);
    void mergeFromFile(const QString &filename);
    void loadFromFile(const QString &filename);

    void search(const QString &text);

    void clear();
    bool isModified() const;

signals:
    void dataCountChanged(int count);

private:
    Vector<Employee> m_data;
    Vector<Employee> m_displayData;
    QStringList m_headers;
    bool m_isModified;
};
