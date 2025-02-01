#pragma execution_character_set("utf-8")
#include "mychat.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyChat w;
    w.show();
    return a.exec();
}
