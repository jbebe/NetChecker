#include <QApplication>

#include "include/NetCheckerWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    NetCheckerWindow dialog;
    dialog.show();
    return app.exec();
}
