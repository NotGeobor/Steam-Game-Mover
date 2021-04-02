#ifndef PATH_H
#define PATH_H

#include <QLineEdit>
#include <QFileDialog>


class Path : public QLineEdit
{
    Q_OBJECT
    using QLineEdit::QLineEdit;

public slots:
    void getPath(bool &saved, std::string path);
};

#endif // PATH_H
