#include <QtWidgets>
#include <QVector>
#include <QCoreApplication>

#ifndef DIAGNOSTIC_HPP
#define DIAGNOSTIC_HPP

class Diagnostic : public QObject {
	Q_OBJECT
private:
	typedef QString (Diagnostic::*function_t)();
	QVector<function_t> func_vec;
	QString separator;
	QString tab;
public slots:
	void onStarted();
signals:
	void Ready();
	void Update(int, const QString);
	void Quit();
public:
	Diagnostic();
	QString getRoutingInfo();
	QString getInterfaceInfo();
	QString getDGPingOutput();
	QString getODPingOutput();
	QString getOsInfo();
	QString getDNSLookup();
};

#endif

