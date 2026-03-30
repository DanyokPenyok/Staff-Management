#include "database.h"
#include <QDataStream>
#include <QTextStream>
#include <QFile>

DataBase::DataBase(QObject *parent) : QAbstractTableModel(parent), m_isModified(false) {
    m_headers << "Фамилия" << "Имя" << "Отчество" << "Должность" << "Отдел" << "Зарплата";
}

int DataBase::rowCount(const QModelIndex &parent) const {//кол во строк
    return m_displayData.size();
}

int DataBase::columnCount(const QModelIndex &parent) const {//кол во колонок
    return m_headers.size();
}

QVariant DataBase::headerData(int section, Qt::Orientation orientation, int role) const {//секция ориентация роль  иекст для шапки
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return m_headers.at(section);
    }
    return QVariant();
}

QVariant DataBase::data(const QModelIndex &index, int role) const{ //отображение ячеек
    if (!index.isValid() || role != Qt::DisplayRole) { //при запросе несуществ строки выводит пустоту. обрабатывается только текст
        return QVariant();
    }
    const Employee &item = m_displayData[index.row()]; //номер строки

    switch (index.column()) {//номер столбца и заполнение
    case 0: return item.getSurname();
    case 1: return item.getName();
    case 2: return item.getPatronymic();
    case 3: return item.getPosition();
    case 4: return item.getDepartment();
    case 5: return item.getSalary();
    default: return QVariant();
    }
}

void DataBase::addEmployee(const Employee &emp) {
    beginInsertRows(QModelIndex(), m_displayData.size(), m_displayData.size());//создаем пустую строку с индексом m_displayData.size()
    m_data.push_back(emp);//в хранилище
    m_displayData.push_back(emp);//отобразить
    endInsertRows();//конец, увеличено колво строк и нарисована
    m_isModified = true;
    emit dataCountChanged(m_data.size());
}

void DataBase::deleteEmployee(int row) {
    if (row < 0 || row >= m_displayData.size())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_data.remove(row);
    m_displayData.remove(row);
    endRemoveRows();//конец, уменьшено колво строк и нарисована
    m_isModified = true;
    emit dataCountChanged(m_data.size());
}

void DataBase::editEmployee(int row, const Employee& emp) {
    if (row < 0 || row >= m_displayData.size())
        return;

    m_data[row] = emp;//перезапись данных
    m_displayData[row] = emp;
    emit dataChanged(index(row,0), index(row,5)); //изменение данных в n строке с 0 по 5 столбец
    m_isModified = true;
}

const Employee& DataBase::getEmployee(int row) const {
    return m_displayData[row];
}

void DataBase::saveToFile(const QString &filename) {
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);

        for (auto it = m_data.begin(); it != m_data.end(); ++it) {
            out << it->getSurname() << "|"
                << it->getName() << "|"
                << it->getPatronymic() << "|"
                << it->getPosition() << "|"
                << it->getDepartment() << "|"
                << it->getSalary() << "\n";
        }
        m_isModified = false;
        emit dataCountChanged(m_data.size());
    }
}

void DataBase::mergeFromFile(const QString &filename) {
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        beginResetModel();

        QTextStream in(&file);

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split("|");
            if (parts.size() == 6) {
                Employee emp(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5]);
                m_data.push_back(emp);
                m_displayData.push_back(emp);
            }
        }
        endResetModel();
        m_isModified = true;
        emit dataCountChanged(m_data.size());
    }
}

void DataBase::loadFromFile(const QString &filename) {
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        beginResetModel();
        m_data.clear();
        m_displayData.clear();

        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split("|");
            if (parts.size() == 6) {
                Employee emp(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5]);
                m_data.push_back(emp);
                m_displayData.push_back(emp);
            }
        }
        endResetModel();
        m_isModified = false;
        emit dataCountChanged(m_data.size());
    }
}

void DataBase::search(const QString &text) {
    beginResetModel();
    m_displayData.clear();
    if (text.isEmpty()) {
        for (auto const& item : m_data) {
            m_displayData.push_back(item);
        }
    } else {
        for (auto it = m_data.begin(); it != m_data.end(); ++it) {
            if (it->getSurname().contains(text, Qt::CaseInsensitive)||
                it->getPosition().contains(text, Qt::CaseInsensitive) ||
                it->getDepartment().contains(text, Qt::CaseInsensitive)) {
                m_displayData.push_back(*it);
            }
        }
    }
    endResetModel();
}

void DataBase::clear() {
    beginResetModel();
    m_data.clear();
    m_displayData.clear();
    endResetModel();
    m_isModified = false;
    emit dataCountChanged(0);
}

bool DataBase::isModified() const {
    return m_isModified;
}
