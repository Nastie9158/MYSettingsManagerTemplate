#include <QApplication>
#include "settingswindow.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    qDebug()<< "App is starting";
    QApplication app(argc, argv);
    SettingsWindow window;
    window.show();
    return app.exec();
}
