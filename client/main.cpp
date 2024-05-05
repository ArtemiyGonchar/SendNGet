#include "client.h"
#include <QFile>
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString path = QDir::currentPath();
    QString choppedPath;
    for (int i = 0; i < path.indexOf("SendNGet") + 8; i++){
        choppedPath += path[i];
    }
    choppedPath += "/client/Adaptic.qss";

    QFile styleSheetFile(choppedPath);
    QDir::currentPath();
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    a.setStyleSheet(styleSheet);

    Client w;
    w.show();
    return a.exec();
}
