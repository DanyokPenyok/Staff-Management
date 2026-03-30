#pragma once
#include <QMainWindow>
#include <QCloseEvent>
#include <QLabel>
#include <QLineEdit>
#include <QTableView>
#include "database.h" // Убедитесь, что этот файл существует

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onNew();
    void onAdd();
    void onDelete();
    void onEdit();
    bool onSave();
    void onLoad();
    void onMerge();
    void onSearch(const QString &text);
    void onAbout();

private:
    DataBase *db;
    QTableView *tableView;
    QLineEdit *searchEdit;

    void createMenu();
    void createToolbar();

    bool maybeSave();

    void updateStatusLabel(int count);
    QLabel *statusLabel;
};
