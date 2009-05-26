#include <QApplication>

#include "idupdater.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    idupdater form("http://kaidokert.com/work/updater/");
    form.show();
    return app.exec();
}
