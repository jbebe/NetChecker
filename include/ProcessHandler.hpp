#include <QObject>
#include <QProcess>
#include <QStringList>

#ifndef PROCESSHANDLER_HPP
#define PROCESSHANDLER_HPP

class ProcessHandler : protected QProcess {
	Q_OBJECT
public:
	ProcessHandler(QProcess::OpenMode mode);
public:
	QString exec(QString commands);
};

#endif

