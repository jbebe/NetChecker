#include <QString>
#include <QMessageBox>

#ifndef PRIVILEGES_HPP
#define PRIVILEGES_HPP

class Privileges {

public:
	static void checkPrivileges(QWidget*, const QString, const QString);

	static bool isRoot();
};

#endif // PRIVILEGES_HPP
