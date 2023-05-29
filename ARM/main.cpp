#include "arm.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ARM w;
    w.setWindowIcon(QIcon(":/myIcon.ico"));
    w.setWindowTitle("АРМ классного руководителя");
    QPalette Pal(QColor(160, 204, 122));
    w.setPalette(Pal);
    w.setAutoFillBackground(true);
    w.show();
    return a.exec();
}
