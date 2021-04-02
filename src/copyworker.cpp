#include "copyworker.h"


void CopyWorker::copy(std::string ob, std::string loc, std::string game, std::string num, bool cSize)
{
    std::error_code ec;

    if (cSize) {
        std::filesystem::create_directory(loc + "\\steamapps\\common\\" + game, ec);

        if (ec)
            emit emitMessage(QString::fromLocal8Bit(ec.message()));

        std::filesystem::recursive_directory_iterator gameDir(ob + "\\steamapps\\common\\" + game, ec);

        if (ec)
            emit emitMessage(QString::fromLocal8Bit(ec.message()));

        long long currentbytes = 0;
        long long bytes = 0;
        int progress = 0;
        std::string path = "";
        std::string extend = "...";
        std::vector<std::filesystem::path> paths;

        for (auto &p : gameDir) {
            paths.push_back(p.path());
            bytes += std::filesystem::file_size(p.path());
        }

        emit emitRange(0, paths.size());

        for (auto &p : paths) {
            std::stringstream ss;
            ss << p;
            path = ss.str();
            path.replace(0, ob.size() + 2, loc);
            path.pop_back();
            if (std::filesystem::is_directory(p)) {
                std::filesystem::create_directory(path, ec);
                if (ec) {
                    emit emitMessage(QString::fromLocal8Bit(ec.message()));
                    ec.clear();
                } else {
                    emit emitMessage(QString::fromLocal8Bit(extend + path.substr(path.size() - 40, 40)));
                }
                ++progress;
                emit emitProgress(progress);
            } else {
                std::filesystem::copy(p, path, ec);
                if (ec) {
                    emit emitMessage(QString::fromLocal8Bit(ec.message()));
                    ec.clear();
                } else {
                    emit emitMessage(QString::fromLocal8Bit(extend + path.substr(path.size() - 40, 40)));
                }
                ++progress;
                emit emitProgress(progress);

                currentbytes += std::filesystem::file_size(p);
                if (bytes >= 1000000000) {
                    std::stringstream ss1;
                    ss1 << currentbytes / 1000000000.0;
                    std::string sCurrentBytes = ss1.str();
                    std::stringstream ss2;
                    ss2 << bytes / 1000000000.0;
                    std::string sTotalBytes = ss2.str();
                    emit emitBytes(QString::fromLocal8Bit(sCurrentBytes + " of " + sTotalBytes + " GB Transferred"));
                } else if (bytes >= 1000000) {
                    std::stringstream ss1;
                    ss1 << currentbytes / 1000000.0;
                    std::string sCurrentBytes = ss1.str();
                    std::stringstream ss2;
                    ss2 << bytes / 1000000.0;
                    std::string sTotalBytes = ss2.str();
                    emit emitBytes(QString::fromLocal8Bit(sCurrentBytes + " of " + sTotalBytes + " MB Transferred"));
                } else if (bytes >= 1000) {
                    std::stringstream ss1;
                    ss1 << currentbytes / 1000.0;
                    std::string sCurrentBytes = ss1.str();
                    std::stringstream ss2;
                    ss2 << bytes / 1000.0;
                    std::string sTotalBytes = ss2.str();
                    emit emitBytes(QString::fromLocal8Bit(sCurrentBytes + " of " + sTotalBytes + " KB Transferred"));
                } else {
                    std::stringstream ss1;
                    ss1 << currentbytes;
                    std::string sCurrentBytes = ss1.str();
                    std::stringstream ss2;
                    ss2 << bytes;
                    std::string sTotalBytes = ss2.str();
                    emit emitBytes(QString::fromLocal8Bit(sCurrentBytes + " of " + sTotalBytes + " B Transferred"));
                }
            }
        }
        std::string manifest = "appmanifest_" + num + ".acf";
        std::filesystem::copy(ob + "\\steamapps\\" + manifest, loc + "\\steamapps\\" + manifest, ec);

        emit emitMessage(QString::fromLocal8Bit("Finished"));
    } else {
        emit emitBytes(QString::fromLocal8Bit("Unknown Bytes Transferred"));

        std::filesystem::create_directory(loc + "\\steamapps\\common\\" + game, ec);

        if (ec)
            emit emitMessage(QString::fromLocal8Bit(ec.message()));

        std::filesystem::recursive_directory_iterator gameDir(ob + "\\steamapps\\common\\" + game, ec);

        if (ec)
            emit emitMessage(QString::fromLocal8Bit(ec.message()));

        int progress = 0;
        std::string path = "";
        std::string extend = "...";
        std::vector<std::filesystem::path> paths;

        for (auto &p : gameDir) {
            paths.push_back(p.path());
        }

        emit emitRange(0, paths.size());

        for (auto &p : paths) {
            std::stringstream ss;
            ss << p;
            path = ss.str();
            path.replace(0, ob.size() + 2, loc);
            path.pop_back();
            if (std::filesystem::is_directory(p)) {
                std::filesystem::create_directory(path, ec);
                if (ec) {
                    emit emitMessage(QString::fromLocal8Bit(ec.message()));
                    ec.clear();
                } else {
                    emit emitMessage(QString::fromLocal8Bit(extend + path.substr(path.size() - 40, 40)));
                }
                ++progress;
                emit emitProgress(progress);
            } else {
                std::filesystem::copy(p, path, ec);
                if (ec) {
                    emit emitMessage(QString::fromLocal8Bit(ec.message()));
                    ec.clear();
                } else {
                    emit emitMessage(QString::fromLocal8Bit(extend + path.substr(path.size() - 40, 40)));
                }
                ++progress;
                emit emitProgress(progress);
            }
        }
    }
    std::string manifest = "appmanifest_" + num + ".acf";
    std::filesystem::copy(ob + "\\steamapps\\" + manifest, loc + "\\steamapps\\" + manifest, ec);

    emit emitMessage(QString::fromLocal8Bit("Finished"));
}
