#include <QApplication>

#include <include/NetCheckerWindow.hpp>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	NetCheckerWindow ncw;
	ncw.show();
	return app.exec();
}
