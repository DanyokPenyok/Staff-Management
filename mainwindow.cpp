#include "mainwindow.h" // <-- Критично! Объявляет класс MainWindow
#include "input_dialog.h" // <-- Критично! Объявляет InputDialog
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox> // <-- Объявляет QMessageBox
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QStyle>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QModelIndex> // Добавлено явно, на случай старых версий Qt, чтобы решить ошибку 'QModelIndex'

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    db = new DataBase(this);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(new QLabel("Поиск (Фамилия, Должность, Отдел):"));
    searchEdit = new QLineEdit();
    searchLayout->addWidget(searchEdit);
    layout->addLayout(searchLayout);

    tableView = new QTableView();
    tableView->setModel(db);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(tableView);

    statusBar()->show();
    statusLabel = new QLabel("Загрузите или создайте БД.");
    statusBar()->addPermanentWidget(statusLabel);

    createMenu();
    createToolbar();

    connect(searchEdit, &QLineEdit::textChanged, this, &MainWindow::onSearch);
    connect(db, &DataBase::dataCountChanged, this, &MainWindow::updateStatusLabel);

    resize(900, 600);
}

MainWindow::~MainWindow() {}

bool MainWindow::maybeSave() {
    if (!db->isModified()) {
        return true;
    }

    const QMessageBox::StandardButton ret = QMessageBox::warning(this, "Приложение",
                                                                 "Документ был изменен.\n"
                                                                 "Хотите сохранить изменения?",
                                                                 QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    switch (ret) {
    case QMessageBox::Save:
        return onSave();
    case QMessageBox::Cancel:
        return false;
    default:
        return true;
    }
}

void MainWindow::onNew() {
    if (maybeSave()) {
        db->clear();
    }
}

void MainWindow::createMenu() {
    QMenu *fileMenu = menuBar()->addMenu("Файл");

    fileMenu->addAction(style()->standardIcon(QStyle::SP_FileIcon), "Создать", this, &MainWindow::onNew);
    fileMenu->addAction(style()->standardIcon(QStyle::SP_DialogOpenButton),"Открыть", this, &MainWindow::onLoad);
    fileMenu->addAction(style()->standardIcon(QStyle::SP_DialogSaveButton),"Сохранить", this, &MainWindow::onSave);
    fileMenu->addAction(style()->standardIcon(QStyle::SP_DriveNetIcon), "Объединить", this, &MainWindow::onMerge);
    fileMenu->addSeparator();
    fileMenu->addAction(style()->standardIcon(QStyle::SP_FileDialogBack),"Выйти", this, &MainWindow::close);

    QMenu *recordMenu = menuBar()->addMenu("Сотрудник");
    recordMenu->addAction(style()->standardIcon(QStyle::SP_FileDialogNewFolder), "Добавить", this, &MainWindow::onAdd);
    recordMenu->addAction(style()->standardIcon(QStyle::SP_FileDialogDetailedView), "Редактировать", this, &MainWindow::onEdit);
    recordMenu->addAction(style()->standardIcon(QStyle::SP_TrashIcon), "Удалить", this, &MainWindow::onDelete);

    QMenu *helpMenu = menuBar()->addMenu("Помощь");
    helpMenu->addAction(style()->standardIcon(QStyle::SP_MessageBoxInformation), "О программе", this, &MainWindow::onAbout);
}

void MainWindow::createToolbar() {
    QToolBar *toolbar = addToolBar("Инструменты");

    toolbar->addAction(style()->standardIcon(QStyle::SP_FileIcon), "Создать", this, &MainWindow::onNew);
    toolbar->addAction(style()->standardIcon(QStyle::SP_FileDialogNewFolder), "Добавить", this, &MainWindow::onAdd);
    toolbar->addAction(style()->standardIcon(QStyle::SP_FileDialogDetailedView), "Редактировать", this, &MainWindow::onEdit);
    toolbar->addAction(style()->standardIcon(QStyle::SP_TrashIcon), "Удалить", this, &MainWindow::onDelete);
    toolbar->addSeparator();
    toolbar->addAction(style()->standardIcon(QStyle::SP_DialogOpenButton),"Открыть", this, &MainWindow::onLoad);
    toolbar->addAction(style()->standardIcon(QStyle::SP_DialogSaveButton),"Сохранить", this, &MainWindow::onSave);
    toolbar->addAction(style()->standardIcon(QStyle::SP_DriveNetIcon), "Объединить", this, &MainWindow::onMerge);
    toolbar->addSeparator();
    toolbar->addAction(style()->standardIcon(QStyle::SP_DialogHelpButton),"О программе",this,&MainWindow::onAbout);
}

void MainWindow::onAdd() {
    InputDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
        db->addEmployee(dialog.getEmployee());
}

void MainWindow::onDelete() {
    QModelIndex index = tableView->currentIndex();
    if (!index.isValid())
        return;
    auto answer =
        QMessageBox::question(this, "Удалить", "Вы уверены, что хотите удалить запись о сотруднике?");

    if (answer == QMessageBox::Yes)
        db->deleteEmployee(index.row());
}

void MainWindow::onEdit() {
    QModelIndex index = tableView->currentIndex();
    if (!index.isValid()) return;

    InputDialog dialog(this);
    dialog.setEmployee(db->getEmployee(index.row()));

    if (dialog.exec() == QDialog::Accepted) {
        db->editEmployee(index.row(), dialog.getEmployee());
    }
}

void MainWindow::updateStatusLabel(int count) {
    if (count == 0) {
        statusLabel->setText("База данных пуста.");
    } else {
        statusLabel->setText(QString("Всего сотрудников: %1").arg(count));
    }
}

bool MainWindow::onSave() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить БД", "", "Text Files (*.txt);;All Files (*)");
    if (!fileName.isEmpty()) {
        if (!fileName.endsWith(".txt")) fileName += ".txt";
        db->saveToFile(fileName);
        return true;
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::onMerge() {
    QString filename = QFileDialog::getOpenFileName(this, "Соединить БД", "", "Text Files (*.txt);;All Files (*)");
    if (!filename.isEmpty()) {
        db->mergeFromFile(filename);
    }
}

void MainWindow::onLoad() {
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, "Открыть БД", "", "Text Files (*.txt);;All Files (*)");
        if (!fileName.isEmpty()) {
            db->loadFromFile(fileName);
        }
    }
}

void MainWindow::onSearch(const QString &text) {
    db->search(text);
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "О проекте", "Курсовая работа по ООП\n"
                                          "Тема: Реализация БД\n"
                                          "Автор: Хохлов Даня");
}
