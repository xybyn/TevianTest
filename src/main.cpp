#include <QApplication>
#include "AuthWindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AuthWindow w;
    w.show();
    int status = a.exec();
    return status;
}
