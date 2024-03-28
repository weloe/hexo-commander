#include "config.h"
#include "ui_config.h"

QString hexoPath = settings.value("hexoPath").toString();
QString workingDir = settings.value("workingDir").toString();
QString postPath = settings.value("postPath").toString();
QString scaffoldsPath = settings.value("scaffoldsPath").toString();



Config::Config(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Config)
{
    ui->setupUi(this);
    ui->hexoPathEdit->setText(hexoPath);
    ui->workingDirEdit->setText(workingDir);
    ui->postPathEdit->setText(postPath);
    ui->scaffoldsPathEdit->setText(scaffoldsPath);
}

Config::~Config()
{
    delete ui;
}

void Config::on_saveButton_clicked()
{
    check_save(ui->hexoPathLabel->text(), ui->hexoPathEdit->text());
    check_save(ui->workingDirLabel->text(), ui->workingDirEdit->text());
    check_save(ui->postPathLabel->text(), ui->postPathEdit->text());
    check_save(ui->scaffoldsPathLabel->text(), ui->scaffoldsPathEdit->text());

    hexoPath = settings.value("hexoPath").toString();
    workingDir = settings.value("workingDir").toString();
    postPath = settings.value("postPath").toString();
    scaffoldsPath = settings.value("scaffoldsPath").toString();
    this->close();
}

bool Config::check_config() {
    if (hexoPath.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "hexoPath cannot be empty, please set config.", QMessageBox::Ok);
        return false;
    }
    if (workingDir.isEmpty())
    {
        QMessageBox::warning(this, "Warning",  "workingDir cannot be empty, please set config.", QMessageBox::Ok);
        return false;
    }
    if (postPath.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "postPath cannot be empty, please set config.", QMessageBox::Ok);
        return false;
    }
    if (scaffoldsPath.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "scaffoldsPath cannot be empty, please set config.", QMessageBox::Ok);
        return false;
    }
    return true;
}

QString Config::getConfig(const QString& key)
{
   return settings.value(key).toString();
}


void Config::check_save(const QString& text,const QString& value)
{
    if (value.isEmpty())
    {
        QMessageBox::warning(this, "Warning", text + " cannot be empty.", QMessageBox::Ok);
        return;
    }
    settings.setValue(text, value);
    qDebug() << "save " << value;
}


void Config::on_pushButton_clicked()
{
    this->close();
}

void Config::on_hexoPathAutoButton_clicked()
{
    QApplication::setOverrideCursor(Qt::WaitCursor); // 设置鼠标光标为等待状态

    QString userDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QStringList possiblePaths = {userDir + "/AppData","C:/Program Files/Hexo"}; // 常见安装路径示例
    qDebug() << "User's directory is: " << userDir;

    QString fileName = "hexo.cmd";
    QStringList searchedPaths;

    // 先搜索可能目录
    for (const QString &dir : possiblePaths) {
        QString filePath = searchForFile(dir, fileName);
        searchedPaths << dir; // 记录已搜索过的路径
        if (!filePath.isEmpty()) {
            qDebug() << "Found" << fileName << "at:" << filePath;
            ui->hexoPathEdit->setText(filePath);
            QApplication::restoreOverrideCursor(); // 恢复正常鼠标光标
            return;
        }
    }

    // 全盘搜索
    QFileInfoList drives = QDir::drives();

    for (const QFileInfo &folderInfo : drives) {
        QString folderPath = folderInfo.absoluteFilePath();
        if (!searchedPaths.contains(folderPath)) { // 避开已搜索过的路径
            QString filePath = searchForFile(folderPath, fileName);
            if (!filePath.isEmpty()) {
                qDebug() << "Found" << fileName << "at:" << filePath;
                ui->hexoPathEdit->setText(filePath);
                QApplication::restoreOverrideCursor(); // 恢复正常鼠标光标
                return;
            }
        }
        QString filePath = searchForFile(folderInfo.absoluteFilePath(), fileName);
        if (!filePath.isEmpty()) {
            qDebug() << "Found" << fileName << "at:" << filePath;
            ui->hexoPathEdit->setText(filePath);
            QApplication::restoreOverrideCursor(); // 恢复正常鼠标光标
            return;
        }
    }

    // 如果未找到文件，则恢复正常鼠标光标，并弹出警告对话框
    QApplication::restoreOverrideCursor();
    QMessageBox::warning(this, "Warning", "hexo.cmd not found.", QMessageBox::Ok);
}

QString Config::searchForFile(const QString &startPath, const QString &fileName)
{
    QDirIterator it(startPath, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        if (it.fileName() == fileName) {
            return it.filePath(); // 找到文件时返回绝对路径
        }
    }

    return ""; // 如果在当前目录或子目录中未找到文件，则返回空字符串
}
