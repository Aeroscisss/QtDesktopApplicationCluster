#include <QtWidgets/QApplication>
#include "CustomQApp.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    CustomQApp app(argc, argv);
    if (!app.initialize()) {
		qCritical() << "app initialization fail, quitting...";
		return -1;
    }
    else {
		qDebug() << "app is running";
        app.showIntroWindow();
		return app.exec();
    }
}
