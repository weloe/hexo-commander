#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QWidget>
#include <QtWidgets>
#include <config.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QProcess* process;
    void executeHexoCommand(const QStringList &command);
    void executeHexoCommand(const QString &command);
    void terminateProcessByPort(int portNumber);
    void loadComboBoxItems();
    void loadPostComboBoxItems();
    void loadComboBoxItems(const QString &folderPath, QComboBox *box);
private:
    Ui::MainWindow *ui;

private slots:
    void on_cleanButton_clicked();

    void on_generateButton_clicked();

    void on_deployButton_clicked();

    void on_cleandButton_clicked();

    void on_createButton_clicked();

    void on_serverButton_clicked();
    void on_pushButton_clicked();
    void on_scaffoldsComboBox_activated(int index);
    void on_configButton_clicked();
    void on_openPostButton_clicked();
    void on_postClearButton_clicked();
    void on_scaffoldsClearButton_clicked();
};

#endif // MAINWINDOW_H
