#ifndef CONFIG_H
#define CONFIG_H

#include <QMainWindow>
#include <QtWidgets>


extern QString hexoPath;
extern QString workingDir;
extern QString postPath;
extern QString scaffoldsPath;
//static QString hexoPath = "C:/Users/psyco/AppData/Roaming/npm/hexo.cmd";
//static QString workingDir = "D:/GithubRepository/novel_t";
//static QString postPath = "D:/GithubRepository/novel_t/source/_posts/";
//static QString scaffoldsPath = "D:/GithubRepository/novel_t/scaffolds/";
static QSettings settings("weloe","hexo-commander");
namespace Ui {
class Config;
}

class Config : public QWidget
{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = nullptr);
    ~Config();

    void check_save(const QString &text, const QString &value);
    bool check_config();
    QString getConfig(const QString &key);
private slots:
    void on_saveButton_clicked();

    void on_pushButton_clicked();

public:
    Ui::Config *ui;
};


#endif // CONFIG_H
