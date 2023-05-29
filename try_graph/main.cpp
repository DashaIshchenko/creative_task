#include "dialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;

    QFont font(":/fonts/MinecraftRegular-Bmg3.otf");
    font.setStyleHint(QFont::Monospace);
    QApplication::setFont(font);

    w.setWindowTitle("Граф и ЗК");
    QPalette Pal(QColor(204, 169, 245));
    w.setPalette(Pal);
    w.setAutoFillBackground(true);
    w.setWindowIcon(QIcon(":/myIcon.ico"));
    w.show();
    return a.exec();
}
