#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(QString("OpenCV Histogram Equalization by I Nyoman Bayu V %1:%2").arg(CV_MAJOR_VERSION).arg(CV_MINOR_VERSION));
    w.show();

    return a.exec();
}
