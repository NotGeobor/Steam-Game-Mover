#ifndef COPYWORKER_H
#define COPYWORKER_H

#include <filesystem>
#include <QListWidgetItem>
#include <QObject>


class CopyWorker : public QObject
{
    Q_OBJECT

public slots:
    void copy(std::string ob, std::string loc, std::string game, std::string num, bool cSize);

signals:
    void emitProgress(int value);
    void emitRange(int min, int max);
    void emitMessage(QString message);
    void emitBytes(QString bytes);
};

#endif // COPYWORKER_H
