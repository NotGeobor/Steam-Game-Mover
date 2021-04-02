#include "path.h"


void Path::getPath(bool &saved, std::string path)
{
    setText(QFileDialog::getExistingDirectory(this, "Choose a Directory", QString::fromStdString(path)));
    saved = false;
}
