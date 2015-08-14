#include <QString>
#include <QMessageBox>

#include <include/Globals.hpp>
#include <include/ProcessHandler.hpp>
#include <include/Privileges.hpp>

#ifndef PRIVILEGES_CPP
#define PRIVILEGES_CPP

void Privileges::checkPrivileges(QWidget *parent, const QString title, const QString message){
	if (!isRoot()){
		QMessageBox::warning(parent, title, message, QMessageBox::Ok, QMessageBox::NoButton);
		parent->close();
	}
}

#if defined(Q_OS_WIN)
	#include <windows.h>
	bool Privileges::isRoot(){
		BOOL fRet = FALSE;
		HANDLE hToken = NULL;
		if( OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)){
			TOKEN_ELEVATION Elevation;
			DWORD cbSize = sizeof(TOKEN_ELEVATION);
			if(GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation),&cbSize)){
				fRet = Elevation.TokenIsElevated;
			}
		}
		if(hToken){
			CloseHandle(hToken);
		}
		return fRet;
	}
#elif defined(Q_OS_LINUX)
	#include <unistd.h>
	bool Privileges::isRoot(){
		return !(getuid() && geteuid());
	}
#elif defined(Q_OS_OSX)
	#error "OSX is not supported yet!"
#else
	#error "This IS is not supported yet!"
#endif

#endif // PRIVILEGES_CPP

