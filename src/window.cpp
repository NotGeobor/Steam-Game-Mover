#include "window.h"


Window::Window(QWidget *parent) : QWidget(parent)
{
    if (!std::filesystem::exists(local + "\\Geobor\\Steam Game Mover\\config.json")) {
        if (!std::filesystem::exists(local + "\\Geobor"))
            std::filesystem::create_directory(local + "\\Geobor");
        if (!std::filesystem::exists(local + "\\Geobor\\Steam Game Mover"))
            std::filesystem::create_directory(local + "\\Geobor\\Steam Game Mover");

        std::ofstream wFile(local + "\\Geobor\\Steam Game Mover\\config.json", std::ios::out);

        nlohmann::json newWrite =
        {
            {"Steam Path", "C:\\Program Files (x86)\\Steam"},
            {"Backup Path", "Z:\\Steam Backup"},
            {"Calculate size", true}
        };

        wFile << std::setw(4) << newWrite << '\n';
        wFile.close();
    }

    if (!std::filesystem::exists(local + "\\Geobor\\Steam Game Mover\\games.json")) {
        if (!std::filesystem::exists(local + "\\Geobor"))
            std::filesystem::create_directory(local + "\\Geobor");
        if (!std::filesystem::exists(local + "\\Geobor\\Steam Game Mover"))
            std::filesystem::create_directory(local + "\\Geobor\\Steam Game Mover");

        std::ofstream wFile(local + "\\Geobor\\Steam Game Mover\\games.json", std::ios::out);

        nlohmann::ordered_json newWrite = defaultgames();

        wFile << std::setw(4) << newWrite << '\n';
        wFile.close();
    }

    std::ifstream rFile(local + "\\Geobor\\Steam Game Mover\\config.json", std::ios::in);
    nlohmann::json jsonIn = nlohmann::ordered_json::parse(rFile);
    rFile.close();
    rFile.open(local + "\\Geobor\\Steam Game Mover\\games.json", std::ios::in);
    games = nlohmann::json::parse(rFile);
    rFile.close();


    sPath = jsonIn.at("Steam Path");
    bPath = jsonIn.at("Backup Path");
    cSize = jsonIn.at("Calculate size");

    cBox = new QCheckBox("Calculate Game Size", this);
    lGame = new QLabel("Select Game:", this);
    sGame = new QComboBox(this);
    sLabel = new QLabel("Path to Steam library:", this);
    sDir = new Path(this);
    sBrowse = new QPushButton("Browse", this);
    bLabel = new QLabel("Path to backup Steam library:", this);
    bDir = new Path(this);
    bBrowse = new QPushButton("Browse", this);
    mButton = new QPushButton("Move", this);
    lBytes = new QLabel("Waiting...", this);
    pBar = new QProgressBar(this);
    status = new QLabel("", this);
    copyWorker = new CopyWorker;

    sDir->setText(QString::fromStdString(sPath));
    bDir->setText(QString::fromStdString(bPath));
    if (cSize)
        cBox->setCheckState(Qt::Checked);
    for (nlohmann::ordered_json::iterator it = games.begin(); it != games.end(); ++it) {
        std::stringstream ss;
        ss << it.key();
        sGame->addItem(QString::fromLocal8Bit(ss.str()));
    }

    cBox->setGeometry(10, 10, 300, 20);
    lGame->setGeometry(10, 40, 300, 20);
    sGame->setGeometry(10, 70, 300, 30);
    sLabel->setGeometry(10, 110, 300, 20);
    sDir->setGeometry(10, 140, 200, 30);
    sBrowse->setGeometry(220, 140, 90, 30);
    bLabel->setGeometry(10, 180, 300, 20);
    bDir->setGeometry(10, 210, 200, 30);
    bBrowse->setGeometry(220, 210, 90, 30);
    mButton->setGeometry(110, 250, 100, 60);
    lBytes->setGeometry(10, 320, 300, 20);
    pBar->setGeometry(10, 350, 300, 30);
    pBar->setValue(0);
    status->setGeometry(10, 390, 300, 20);
    copyWorker->moveToThread(&copyThread);
    copyThread.start();

    QObject::connect(sBrowse, SIGNAL(clicked()), this, SLOT(sSetPath()));
    QObject::connect(bBrowse, SIGNAL(clicked()), this, SLOT(bSetPath()));
    QObject::connect(this, &Window::emitSSetPath, sDir, &Path::getPath);
    QObject::connect(this, &Window::emitBSetPath, bDir, &Path::getPath);
    QObject::connect(cBox, SIGNAL(stateChanged(int)), this, SLOT(stateChanged()));
    QObject::connect(mButton, SIGNAL(clicked()), this, SLOT(move()));
    QObject::connect(this, &Window::emitPath, copyWorker, &CopyWorker::copy);
    QObject::connect(copyWorker, SIGNAL(emitProgress(int)), pBar, SLOT(setValue(int)));
    QObject::connect(copyWorker, SIGNAL(emitRange(int,int)), pBar, SLOT(setRange(int,int)));
    QObject::connect(copyWorker, SIGNAL(emitMessage(QString)), status, SLOT(setText(QString)));
    QObject::connect(copyWorker, SIGNAL(emitBytes(QString)), lBytes, SLOT(setText(QString)));
}


void Window::move()
{
    emit emitPath(bDir->text().toLocal8Bit().constData(), sDir->text().toLocal8Bit().constData(), sGame->currentText().toLocal8Bit().constData(), games.at(sGame->currentText().toLocal8Bit().constData()), cBox->isChecked());
    if (!saved)
        saveJson();
}


void Window::sSetPath()
{
    emit emitSSetPath(saved, sDir->text().toLocal8Bit().constData());
}


void Window::bSetPath()
{
    emit emitBSetPath(saved, bDir->text().toLocal8Bit().constData());
}


void Window::stateChanged()
{
    saved = !saved;
}


void Window::saveJson()
{
    std::ofstream wFile(local + "\\Geobor\\Steam Game Mover\\config.json", std::ios::out);

    nlohmann::json tempWrite =
    {
        {"Steam Path", sDir->text().toLocal8Bit().constData()},
        {"Backup Path", bDir->text().toLocal8Bit().constData()},
        {"Calculate size", cBox->isChecked()}
    };

    wFile << std::setw(4) << tempWrite << '\n';
    wFile.close();
    saved = true;
}

