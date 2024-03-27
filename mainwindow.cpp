#include "mainwindow.h"
#include "ui_mainwindow.h"


// 在头文件 MainWindow.h 中添加必要的包含和成员变量
#include <QThreadPool>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include "ui_config.h"
#include "config.h"

Config * configWidget;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // 在主程序入口处或者构造函数中注册元类型
    qRegisterMetaType<QTextBlock>("QTextBlock");
    qRegisterMetaType<QTextCursor>("QTextCursor");
    ui->setupUi(this);
    ui->autoOpen->setChecked(true);
    process = new QProcess(this);

    connect(process, &QProcess::readyRead, [=](){
        QByteArray output = process->readAll().trimmed(); // 移除末尾空白字符
        if (!output.isEmpty()) { // 只有在输出非空时才追加到 plainTextEdit
            ui->plainTextEdit->appendPlainText(QString::fromLocal8Bit(output));
        }
    });
    loadComboBoxItems();

    QMenu *configMenu = menuBar()->addMenu("File");

    QAction *openAction = new QAction("Setting", this);
    configMenu->addAction(openAction);

    connect(openAction, &QAction::triggered, this, &MainWindow::on_configButton_clicked);

    configWidget = new Config();
    connect(configWidget->ui->saveButton, &QPushButton::clicked, this, [this](){ loadComboBoxItems(); });
}



void MainWindow::on_scaffoldsComboBox_activated(int index)
{
    qDebug() << "on_scaffoldsComboBox_activated";

}

void MainWindow::loadComboBoxItems()
{
    loadComboBoxItems(scaffoldsPath);
    ui->scaffoldsComboBox->setCurrentIndex(ui->scaffoldsComboBox->count() - 1);
}

void MainWindow::loadComboBoxItems(const QString &folderPath)
{
    qDebug() << "loadComboBoxItems";
    QDir directory(folderPath);
    QStringList files = directory.entryList(QStringList() << "*.md", QDir::Files);

    ui->scaffoldsComboBox->clear(); // 清空下拉框中的选项

    foreach(QString filename, files)
    {
        QFileInfo fileInfo(filename);
        QString nameWithoutExtension = fileInfo.baseName(); // 获取不带后缀的文件名
        ui->scaffoldsComboBox->addItem(nameWithoutExtension); // 将不带后缀的文件名添加到下拉框中
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    if (process && process->state() != QProcess::NotRunning) {
        process->terminate();
    }
    delete process;
}


void MainWindow::on_cleanButton_clicked()
{
    executeHexoCommand("clean");
}

void MainWindow::on_generateButton_clicked()
{

    executeHexoCommand("generate");

}

void MainWindow::on_deployButton_clicked()
{
    executeHexoCommand("deploy");
}

void MainWindow::on_cleandButton_clicked()
{
    executeHexoCommand(QStringList() << "clean" << "&" << hexoPath << "deploy");
}



void MainWindow::on_serverButton_clicked()
{
        ui->serverButton->setEnabled(false);
        if(ui->serverButton->text() == "server") {
            executeHexoCommand(QStringList() << "clean" << "&" << hexoPath << "generate" << "&" << hexoPath << "s");
            ui->serverButton->setText("stop");
            QPalette pal = ui->serverButton->palette();
            pal.setColor(QPalette::ButtonText, Qt::red);    //设置按钮上的字体颜色，理论上可以，实际上就是可以
            ui->serverButton->setPalette(pal);

        }else{
            terminateProcessByPort(4000);
            ui->serverButton->setText("server");
            QPalette pal = ui->serverButton->palette();
            pal.setColor(QPalette::ButtonText, Qt::black);    //设置按钮上的字体颜色，理论上可以，实际上就是可以
            ui->serverButton->setPalette(pal);
            ui->plainTextEdit->appendPlainText("INFO  Stop server.");
        }
        ui->serverButton->setEnabled(true);

}

void MainWindow::on_createButton_clicked()
{
    QString articleName = ui->articleName->text().trimmed(); // 获取输入的文章名，并去除首尾空格

    if(articleName.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Article name cannot be empty. Please enter a valid article name.");
        return;
    }

    QDir dir(postPath);
    QString fileName = ui->articleName->text() + ".md";

    QString filePath = dir.filePath(fileName);

    QFile file(filePath);
    if (!file.exists())
    {
        QString scaffoldName;
        if(ui->scaffoldsEdit->text().isEmpty()){
            scaffoldName = ui->scaffoldsComboBox->currentText();
        }else{
            scaffoldName = ui->scaffoldsEdit->text();
        }

        if(!scaffoldName.isEmpty()) {
           executeHexoCommand(QStringList()<< "new" << scaffoldName << ui->articleName->text());
        }else{
           executeHexoCommand(QStringList() << "new" << ui->articleName->text());
        }
    }else{
        QString message = "Warn file [" + filePath + "] already exists.";
        ui->plainTextEdit->textCursor().insertHtml("\n<font color='red'>" + message + "</font>\n");
    }


    if(ui->autoOpen->isChecked()) {
        process->waitForFinished();
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    }
}



void MainWindow::executeHexoCommand(const QString &command)
{
    executeHexoCommand(QStringList() << command);
}

void MainWindow::executeHexoCommand(const QStringList &commands)
{
    if(!configWidget->check_config()) {
        return;
    }
    if (process->state() == QProcess::Running) {
        QMessageBox::warning(this, "Warning", "Process is already running.");
        return;
    }

    process->setWorkingDirectory(workingDir);
    qDebug() << commands;


    process->start(hexoPath, commands);
}


void MainWindow::on_pushButton_clicked()
{
    ui->plainTextEdit->clear();
}

void MainWindow::terminateProcessByPort(int portNumber)
{
#ifdef Q_OS_WIN // Windows平台
    QProcess killProcess;
    killProcess.start("netstat -ano"); // 使用netstat命令获取端口号对应的进程ID
    killProcess.waitForFinished(-1);
    QString output = killProcess.readAll();
    QStringList lines = output.split("\n");
    foreach (QString line, lines) {
        if (line.contains(QString::number(portNumber))) {
            QStringList tokens = line.simplified().split(" ");
            QString pid = tokens.last();
            QProcess killProcess;
            killProcess.start("taskkill /F /PID " + pid); // 使用taskkill命令终止进程
            killProcess.waitForFinished(-1);
        }
    }
#else
    // 其他操作系统的处理
#endif
}

void MainWindow::on_configButton_clicked()
{

    configWidget->show();
}
