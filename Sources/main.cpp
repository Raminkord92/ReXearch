#include "Headers/mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;

    QString styleFilePath = ":/resources/QSS/globalStyles.css";
    QFile styleFile(styleFilePath);
    styleFile.open(QFile::ReadOnly);
    QString styleSheet = QTextStream(&styleFile).readAll();
    styleFile.close();
    app.setStyleSheet(styleSheet);
    w.show();
    return app.exec();
}

