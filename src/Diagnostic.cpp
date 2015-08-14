#include <QtWidgets>
#include <QVector>
#include <QCoreApplication>
#include <QStringList>
#include <QSysInfo>
#include <QRegExp>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QDnsLookup>

#include <include/Diagnostic.hpp>
#include <include/Globals.hpp>
#include <include/ProcessHandler.hpp>

void Diagnostic::onStarted(){
	double step = 100.0/func_vec.size();
	double i = 1.0;
	foreach (function_t fn, func_vec) {
		emit Update(static_cast<int>(ceil(step*i)), (this->*fn)());
		i += 1.0;
	}
	emit Quit();
}

Diagnostic::Diagnostic()
	: func_vec({
	   &Diagnostic::getOsInfo,
	   &Diagnostic::getInterfaceInfo,
	   &Diagnostic::getRoutingInfo,
	   &Diagnostic::getDGPingOutput,
	   &Diagnostic::getODPingOutput,
	   &Diagnostic::getDNSLookup
	}), tab(2, ' ')
{
	INIT_GLOBALS();
}


QString Diagnostic::getRoutingInfo(){
	ProcessHandler proc(QIODevice::ReadOnly);
	QStringList out;

	out << "- - - Routing - - - \n";

	switch (CURRENT_OS){
	case OS_WINDOWS:
		out << proc.exec("netsh interface ipv4 show route");
		//out << proc.exec("netsh interface ipv6 show route");
		break;
	case OS_LINUX:
		out << proc.exec("netstat -A inet -rn");
		//out << proc.exec("netstat -A inet6 -rn");
		break;
	case OS_OSX:
		out << proc.exec("/usr/sbin/netstat -f inet -rn");
		//out << proc.exec("/usr/sbin/netstat -f inet6 -rn");
		break;
	default:
		static_assert(true, "This OS is not supported");
		break;
	}
	return out.join(separator);
}


QString Diagnostic::getInterfaceInfo(){
	QList<QNetworkInterface> ifList = QNetworkInterface::allInterfaces();
	QStringList out;

	out << "- - - Interface Info - - - \n";
	out << "\n";
	foreach (QNetworkInterface iface, ifList){
		out << QString("%1\n").arg(iface.humanReadableName());
		out << QString("%1Address entries:\n").arg(tab);
		{
			int address_cntr = 0;
			foreach (QNetworkAddressEntry netAddr, iface.addressEntries()){
				out << QString("%1%1entry #%2\n").arg(tab).arg(address_cntr++);
				out << QString("%1%1%1- broadcast addr: %2\n").arg(tab).arg(netAddr.broadcast().toString());
				out << QString("%1%1%1- ip addr: %2\n").arg(tab).arg(netAddr.ip().toString());
				out << QString("%1%1%1- subnetmask: %2\n").arg(tab).arg(netAddr.netmask().toString());
			}
		}
		out << QString("%1- can broadcast? %2\n").arg(tab).arg(iface.flags().testFlag(QNetworkInterface::CanBroadcast)? "yes" : "no");
		out << QString("%1- can multicast? %2\n").arg(tab).arg(iface.flags().testFlag(QNetworkInterface::CanMulticast)? "yes" : "no");
		out << QString("%1- HW address: %2\n").arg(tab).arg(iface.hardwareAddress());
		out << QString("%1- is loopback? %2\n").arg(tab).arg(iface.flags().testFlag(QNetworkInterface::IsLoopBack)? "yes" : "no");
		out << QString("%1- is point-to-point? %2\n").arg(tab).arg(iface.flags().testFlag(QNetworkInterface::IsPointToPoint)? "yes" : "no");
		out << QString("%1- is running? %2\n").arg(tab).arg(iface.flags().testFlag(QNetworkInterface::IsRunning)? "yes" : "no");
		out << QString("%1- is up? %2\n").arg(tab).arg(iface.flags().testFlag(QNetworkInterface::IsUp)? "yes" : "no");
		out << QString("%1- is valid? %2\n").arg(tab).arg(iface.isValid()? "yes" : "no");
		out << QString("%1- internal name: %2\n").arg(tab).arg(iface.name());
	}
	out << QString("Local addresses:\n");
	foreach (QHostAddress hostAddr, QNetworkInterface::allAddresses()){
		out << QString("%1- %2\n").arg(tab).arg(hostAddr.toString());
	}
	return out.join(separator);
}


QString Diagnostic::getDGPingOutput(){
	ProcessHandler proc(QIODevice::ReadOnly);
	QStringList out;

	out << "- - - Pinging Default Gateways - - -\n";

	foreach (QString ip, DEF_GATEWAYS) {
		switch (CURRENT_OS){
		case OS_WINDOWS:
			out << proc.exec(QString("ping -n 4 -w 1 %1").arg(ip));
			break;
		case OS_LINUX:
			out << proc.exec(QString("ping -n -c 4 -W 1 %1").arg(ip));
			break;
		case OS_OSX:
		default:
			static_assert(true, "This OS is not supported");
			break;
		}
	}
	return out.join(separator);
}


QString Diagnostic::getODPingOutput(){
	ProcessHandler proc(QIODevice::ReadOnly);
	QStringList out;

	out << "- - - Pinging Outer Destinations - - -\n";

	foreach (QString ip, OUTER_DEST) {
		switch (CURRENT_OS){
		case OS_WINDOWS:
			out << proc.exec(QString("ping -n 4 -w 1 %1").arg(ip));
			break;
		case OS_LINUX:
			out << proc.exec(QString("ping -c 4 -W 1 %1").arg(ip));
			break;
		case OS_OSX:
			out << proc.exec(QString("ping -c 4 -W 1 %1").arg(ip));
			break;
		default:
			static_assert(true, "This OS is not supported");
			break;
		}
	}
	return out.join(separator);
}


QString Diagnostic::getOsInfo(){
	ProcessHandler proc(QIODevice::ReadOnly);
	QStringList out;
	out << "- - - OS Version - - -\n";

	switch (CURRENT_OS) {
	case OS_WINDOWS:
		{
			/* this pattern removes leading/trailing/middle empty lines
			 * it is tested to work without escaping the backslashes.
			 * if the output is malformed feel free to escape them! */
			QRegExp e("(?:^[\r\n]+)|(?:[\r\n]{2}(?=[\r\n]))|(?:[\r\n]+$)");
			out << proc.exec("wmic os get Caption,CSDVersion /value").replace(e, "");
		}
		break;
	case OS_LINUX:
		out << proc.exec("lsb_release -a");
		break;
	case OS_OSX:
		out << proc.exec("sw_vers");
		break;
	default:
		static_assert(true, "This OS is not supported");
		break;
	}
	return out.join(separator);
}


QString Diagnostic::getDNSLookup(){
	QStringList out;
	out << "- - - DNS Lookup for sch.bme.hu - - -\n";
	QEventLoop dns_event;
	QDnsLookup dns_handle(QDnsLookup::ANY, DNS_ADDR);

	dns_event.connect(&dns_handle, SIGNAL(finished()), &dns_event, SLOT(quit()));

	foreach (QString nameserver, NS_ADDRS) {
		dns_handle.setNameserver(QHostAddress(nameserver));
		dns_handle.lookup();
		dns_event.exec();
		if (dns_handle.error() == QDnsLookup::NoError) {
			out << QString("nameserver: %1\n").arg(nameserver);
			foreach (const QDnsHostAddressRecord &record, dns_handle.hostAddressRecords()) {
				out << QString("%1- %2\n").arg(tab).arg(record.value().toString());
			}
		}
		else {
			out << QString("nameserver: %1 - failed\n").arg(nameserver);
		}
	}
	return out.join(separator);
}
