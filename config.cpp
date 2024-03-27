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
