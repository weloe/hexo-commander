#include "util.h"

util::util()
{

}


bool isDirEmpty(const QString &path,QWidget *w)
{
    QDir dir(path);
    if (!dir.exists()) {
        qDebug() << "Directory does not exist.";
        QMessageBox::warning(w, "Warning", "Directory does not exist.");
        return false;
    }

    QStringList entryList = dir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries);
    return entryList.isEmpty();
}
