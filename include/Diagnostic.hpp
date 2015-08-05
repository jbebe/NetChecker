#include <QCoreApplication>
#include <QStringBuilder>
#include <QStringList>
#include <QTextStream>
#include <QRegExp>
#include <QCoreApplication>
#include <QtNetwork/QNetworkInterface>

#ifndef DIAGNOSTIC_HPP
#define DIAGNOSTIC_HPP

class Diagnostic {
public:
    static QString getRoutingInfo(){
        FILE *cmdHandle;
        QString ipAddr("Default Gateway: ");
#if defined( Q_OS_WIN )
        // original command: route print -4 | findstr /r "[^0-9]0\.0\.0\.0"
        cmdHandle = popen(
            "route print -4 | findstr /r \"[^0-9]0\\.0\\.0\\.0\" ", "r"
        );
#elif defined ( Q_OS_LINUX )
        // a guess: netstat -rn | grep "^0\.0\.0\.0" | awk '{print $2}'
        // w/o awk: netstat -rn | sed -rn 's/^0\.0\.0\.0\s+([^ ]+).*/\1/p'
        cmdHandle = popen(
            "netstat -rn | sed -rn 's/^0\.0\.0\.0\s+([^ ]+).*/\1/p'", "r"
        );
#else
#error "We do not support this version."
#endif
        const int BUFFSIZE = 256;
        char buffer[BUFFSIZE];
        fgets(buffer, BUFFSIZE-1, cmdHandle);

        QRegExp regx("^(?:\\s*(?:0\\.0\\.0\\.0)){2}\\s*([^\\s]+)");
        regx.indexIn(buffer);
        ipAddr.append(regx.capturedTexts()[1]);
        return ipAddr;
    }

    static QString getInterfaceInfo(){
        QList<QNetworkInterface> ifList = QNetworkInterface::allInterfaces();
        QStringList out;
        QString tab("    ");
        foreach (QNetworkInterface iface, ifList){
            out << QString("%1\n").arg(iface.humanReadableName());
            out << QString("%1Address entries:\n").arg(tab);
            foreach (QNetworkAddressEntry netAddr, iface.addressEntries()){
                out << QString("%1%1- broadcast addr: %2\n").arg(tab).arg(netAddr.broadcast().toString());
                out << QString("%1%1- ip addr: %2\n").arg(tab).arg(netAddr.ip().toString());
                out << QString("%1%1- subnetmask: %2\n").arg(tab).arg(netAddr.netmask().toString());
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
        return QString(out.join(' '));
    }

};

#endif

