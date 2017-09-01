#include <QApplication>

#include "mainwindow.h"
#include "global_value.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
#ifdef DEVICE_EVB
    // Change app font family and size to supprot all device.
    QFont appFont = app.font();
    appFont.setPixelSize(font_size);
    app.setFont(appFont);
#endif

    MainWindow w;
#ifdef DEVICE_EVB
    w.showMaximized();
#else
    w.showFullScreen();
#endif
    return app.exec();
}
