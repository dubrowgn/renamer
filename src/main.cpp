#include "renamer-window.h"
#include "QApplication"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    RenamerWindow w;
    w.show();

    return a.exec();
}
