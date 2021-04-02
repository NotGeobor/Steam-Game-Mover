#ifndef WINDOW_H
#define WINDOW_H
#define _CRT_SECURE_NO_WARNINGS

#include <sstream>
#include <fstream>
#include <QPushButton>
#include <QCheckBox>
#include <QProgressBar>
#include <QComboBox>
#include <QThread>
#include <QLabel>
#include "copyworker.h"
#include "path.h"
#include "defaultgames.h"


class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);

    void saveJson();

public slots:
    void move();
    void sSetPath();
    void bSetPath();
    void stateChanged();

signals:
    void emitPath(std::string ob, std::string loc, std::string game, std::string num, bool cSize);
    void emitSSetPath(bool &saved, std::string path);
    void emitBSetPath(bool &saved, std::string path);

private:
    QCheckBox *cBox;
    QLabel *cLabel;
    QLabel *lGame;
    QComboBox *sGame;
    QLabel *sLabel;
    Path *sDir;
    QPushButton *sBrowse;
    QLabel *bLabel;
    Path *bDir;
    QPushButton *bBrowse;
    QPushButton *mButton;
    QLabel *lBytes;
    QLabel *status;
    QProgressBar *pBar;

    QThread copyThread;
    CopyWorker *copyWorker;

    std::string local = getenv("LOCALAPPDATA");
    std::string sPath;
    std::string bPath;
    bool cSize;
    bool saved = true;
    nlohmann::ordered_json games;
};

#endif // WINDOW_H
