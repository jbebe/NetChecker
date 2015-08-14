#include <QObject>
#include <QProcess>
#include <QStringList>

#include <include/ProcessHandler.hpp>

#ifndef PROCESSHANDLER_CPP
#define PROCESSHANDLER_CPP

ProcessHandler::ProcessHandler(QProcess::OpenMode mode): QProcess() {
	setOpenMode(mode);
}
QString ProcessHandler::exec(QString commands){
	start(commands);
	if (!waitForFinished()){
		return tr("Error occured. Command was: %1\n").arg(commands);
	}
	QByteArray ba = readAllStandardOutput();
	return ba;
}

#endif

