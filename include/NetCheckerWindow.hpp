#ifndef DIALOG_H
#define DIALOG_H

#include "Diagnostic.hpp"

#include <QDialog>
#include <QtWidgets>
#include <QFont>

QT_BEGIN_NAMESPACE
class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QTextEdit;
QT_END_NAMESPACE

class NetCheckerWindow : public QDialog
{
	Q_OBJECT
private:
	/* widget structure --> */
	QStackedWidget *global_w; QVBoxLayout *global_l;
		QWidget *welcome_page_w; QHBoxLayout *welcome_page_l;
			QWidget *logo_side_w; QVBoxLayout *logo_side_l;
				QLabel *logo_image_w;
			QWidget *readme_side_w; QVBoxLayout *readme_side_l;
				QTextEdit *text_w;
				QPushButton *next_button_w;
		QWidget *diagnostic_page_w; QVBoxLayout *diagnostic_page_l;
			QGroupBox *progressbar_holder_w; QVBoxLayout *progressbar_holder_l;
				QProgressBar *progressbar_w;
				QLabel *progress_info;
				QLabel *progress_info_time;
			QGroupBox *log_holder_w; QVBoxLayout *log_holder_l;
				QTextEdit *log_w;
			QWidget *button_holder_w; QHBoxLayout *button_holder_l;
				QPushButton* clipboard_button_w;
				QPushButton* save_button_w;
	/* <-- widget structure */
	QTimer *progress_timer;
	int lang;
	Diagnostic *diag_obj;
	QThread *diag_thread;
public:
	NetCheckerWindow();
public slots:
	void nextPage();
	void copyClipboard();
	void saveToFile();
	void changeToHun();
	void changeToEng();
	void incrementTime();
	void finishAction();
	void updateInfos(int, const QString);
private:
	void initStackedWindow();
	void initWelcomePage();
	void initLogoSide();
	void initReadmeSide();
	void initDiagnosticPage();
	void initText();
	enum {
		WINDOW_WIDTH = 380,
		WINDOW_HEIGHT = 420,
		LOGO_WIDTH = 120,
		README_WIDTH = (WINDOW_WIDTH-LOGO_WIDTH)
	};
};

#endif
