#include <QtGlobal>
#include <QStringList>

#ifndef GLOBALS_HPP
#define GLOBALS_HPP

//
// Compile time OS flags
//
enum OS_TYPE {OS_WINDOWS, OS_LINUX, OS_OSX, OS_UNKNOWN};

#if defined(Q_OS_WIN)
	constexpr enum OS_TYPE CURRENT_OS = OS_WINDOWS;
#elif defined(Q_OS_LINUX)
	constexpr enum OS_TYPE CURRENT_OS = OS_LINUX;
#elif defined(Q_OS_OSX)
	constexpr enum OS_TYPE CURRENT_OS = OS_OSX;
#else
	constexpr enum OS_TYPE CURRENT_OS = OS_UNKNOWN;
#endif

//
// Config
//
extern QStringList DEF_GATEWAYS;
extern QStringList OUTER_DEST;
extern QStringList NS_ADDRS;
extern QString DNS_ADDR;

void INIT_GLOBALS();

#endif // GLOBALS_HPP
