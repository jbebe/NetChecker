#include <QSvgRenderer>
#include <QPainter>
#include <QImage>

#include <include/NetCheckerWindow.hpp>
#include <include/Privileges.hpp>
#include <include/Translation.hpp>

NetCheckerWindow::NetCheckerWindow(){
	/* default language is hungarian */
	lang = LANG::HUN;
	setWindowFlags(Qt::WindowCloseButtonHint);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	/* initialize stacked window structure */
	initStackedWindow();
	/* localization for texts in program */
	initText();
}

void NetCheckerWindow::initStackedWindow(){
	global_l = new QVBoxLayout;
	global_w = new QStackedWidget;
	global_l->addWidget(global_w);

	/* remove margins and spacing from global layout */
	global_l->setMargin(0);
	global_l->setSpacing(0);

	initWelcomePage();
	initDiagnosticPage();

	setLayout(global_l);
}

void NetCheckerWindow::initWelcomePage(){
	/* initialization */
	welcome_page_l = new QHBoxLayout;
	welcome_page_w = new QWidget;

	/* remove margins and spacing from welcome page layout */
	welcome_page_l->setMargin(0);
	welcome_page_l->setSpacing(0);

	/* init the two sides: logo and readme */
	initLogoSide();
	initReadmeSide();

	welcome_page_w->setLayout(welcome_page_l);
	global_w->addWidget(welcome_page_w);
}

void NetCheckerWindow::initLogoSide(){
	logo_side_w = new QWidget;
	logo_side_l = new QVBoxLayout;

	logo_side_l->setContentsMargins(9, 9, 0, 0);
	logo_side_w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	logo_side_w->setFixedWidth(LOGO_WIDTH);

	/* logo image at the left */
	logo_image_w = new QLabel;
	QSvgRenderer renderer(tr(":/res/logo.svg"));
	int image_width = LOGO_WIDTH - 9;
	QImage image(image_width, static_cast<int>(1.376888*image_width), QImage::Format_ARGB32);
	image.fill(Qt::transparent);
	QPainter painter(&image);
	renderer.render(&painter);
	QPixmap pix_logo = QPixmap::fromImage(image);
	logo_image_w->setPixmap(pix_logo);
	logo_image_w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	/* flags for choosing language */
	QWidget *flag_holder_w = new QWidget;
	QHBoxLayout *flag_holder_l = new QHBoxLayout;
	QPushButton *flag_eng_w = new QPushButton;
	QPushButton *flag_hun_w = new QPushButton;
	QPixmap pix_flag_eng(":/res/flag_eng.gif");
	QPixmap pix_flag_hun(":/res/flag_hun.gif");
	QIcon icon_eng(pix_flag_eng);
	QIcon icon_hun(pix_flag_hun);
	QSize icon_size(pix_flag_eng.width(), pix_flag_eng.height());

	flag_eng_w->setIconSize(icon_size);
	flag_eng_w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	flag_eng_w->setFixedSize(icon_size.width()+6, icon_size.height()+6);
	flag_eng_w->setIcon(icon_eng);

	flag_hun_w->setIconSize(icon_size);
	flag_hun_w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	flag_hun_w->setFixedSize(icon_size.width()+6, icon_size.height()+6);
	flag_hun_w->setIcon(icon_hun);

	connect(flag_eng_w, SIGNAL(released()), this, SLOT(changeToEng()));
	connect(flag_hun_w, SIGNAL(released()), this, SLOT(changeToHun()));

	flag_holder_l->addWidget(flag_hun_w);
	flag_holder_l->addWidget(flag_eng_w);
	flag_holder_w->setLayout(flag_holder_l);

	logo_side_l->addWidget(logo_image_w);
	logo_side_l->addWidget(flag_holder_w);
	logo_side_w->setLayout(logo_side_l);

	welcome_page_l->addWidget(logo_side_w);
}

void NetCheckerWindow::initReadmeSide(){
	readme_side_w = new QWidget;
	readme_side_l = new QVBoxLayout;

	readme_side_w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	/* textbox at the right */
	text_w = new QTextEdit;
	text_w->setReadOnly(true);

	/* next button */
	next_button_w = new QPushButton;
	next_button_w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	connect(next_button_w, SIGNAL(released()), this, SLOT(nextPage()));

	readme_side_l->addWidget(text_w);
	readme_side_l->addWidget(next_button_w, 0, Qt::AlignRight);
	readme_side_w->setLayout(readme_side_l);

	welcome_page_l->addWidget(readme_side_w);
}

void NetCheckerWindow::initDiagnosticPage(){
	diagnostic_page_w = new QWidget;
	diagnostic_page_l = new QVBoxLayout;

	/* progress bar */
	progressbar_holder_w = new QGroupBox;
	progressbar_holder_l = new QVBoxLayout;
	progressbar_w = new QProgressBar;
	progressbar_w->setRange(0, 100);
	progressbar_w->setAlignment(Qt::AlignHCenter);

	/* progress bar info */
	QWidget *progress_info_holder_w = new QWidget;
	QHBoxLayout *progress_info_holder_l = new QHBoxLayout;
	progress_info = new QLabel;
	progress_info_time = new QLabel;
	progress_info->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	progress_info_time->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	progress_info_time->setFixedWidth(20);
	progress_info_time->setAlignment(Qt::AlignRight);
	progress_info->setAlignment(Qt::AlignRight);

	progress_info_holder_l->addWidget(progress_info);
	progress_info_holder_l->addWidget(progress_info_time, 0, Qt::AlignRight);
	progress_info_holder_l->setContentsMargins(0, 3, 3, 3);

	progress_info_holder_w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	progress_info_holder_w->setLayout(progress_info_holder_l);

	progressbar_holder_w->setLayout(progressbar_holder_l);
	progressbar_holder_l->addWidget(progressbar_w);
	progressbar_holder_l->addWidget(progress_info_holder_w);
	progressbar_holder_w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	/* log */
	log_holder_w = new QGroupBox;
	log_holder_l = new QVBoxLayout;
	log_w = new QTextEdit;
	log_w->setReadOnly(true);
	log_w->setLineWrapMode(QTextEdit::NoWrap);
	log_w->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	log_w->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
	log_holder_w->setLayout(log_holder_l);
	log_holder_l->addWidget(log_w);

	/* buttons */
	button_holder_w = new QWidget;
	button_holder_l = new QHBoxLayout;
	clipboard_button_w = new QPushButton;
	save_button_w = new QPushButton;
	clipboard_button_w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	save_button_w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	clipboard_button_w->setEnabled(false);
	save_button_w->setEnabled(false);

	connect(clipboard_button_w, SIGNAL(released()), this, SLOT(copyClipboard()));
	connect(save_button_w, SIGNAL(released()), this, SLOT(saveToFile()));

	button_holder_l->addWidget(clipboard_button_w);
	button_holder_l->addWidget(save_button_w);
	button_holder_w->setLayout(button_holder_l);

	/* trying to align buttons */
	button_holder_w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	button_holder_l->setContentsMargins(12, 0, 12, 0);

	diagnostic_page_l->addWidget(progressbar_holder_w);
	diagnostic_page_l->addWidget(log_holder_w);
	diagnostic_page_l->addWidget(button_holder_w, 0, Qt::AlignRight);
	diagnostic_page_w->setLayout(diagnostic_page_l);

	global_w->addWidget(diagnostic_page_w);
}

void NetCheckerWindow::initText(){
	/* set program title */
	setWindowTitle(TXT_APP_TITLE[lang]);

	/* set next button label */
	next_button_w->setText(TXT_NEXT_BTN[lang]);

	/* change the readme text but also cache it */
	static QString readme_txt[2] = {QString(), QString()};
	if (readme_txt[0].isEmpty()){
		QFile readme_file_hun(README_URL[LANG::HUN]);
		QFile readme_file_eng(README_URL[LANG::ENG]);
		readme_file_hun.open(QIODevice::ReadOnly);
		readme_file_eng.open(QIODevice::ReadOnly);
		QTextStream readme_file_str_hun(&readme_file_hun);
		QTextStream readme_file_str_eng(&readme_file_eng);
		readme_file_str_hun.setCodec("UTF-8");
		readme_file_str_eng.setCodec("UTF-8");
		readme_txt[LANG::ENG] = readme_file_eng.readAll();
		readme_txt[LANG::HUN] = readme_file_str_hun.readAll();
		readme_file_hun.close();
		readme_file_eng.close();
	}
	text_w->setHtml(readme_txt[lang]);

	/* progress bar and buttons region */
	progress_info->setText(TXT_EST_TIME[lang].append(":"));
	progressbar_holder_w->setTitle(TXT_DIAG_GROUP[lang]);
	log_holder_w->setTitle(TXT_LOG_GROUP[lang]);
	clipboard_button_w->setText(TXT_CLIPB_BTN[lang]);
	save_button_w->setText(TXT_SAVE_BTN[lang]);
}

void NetCheckerWindow::nextPage(){
	/* check for root privileges */
	Privileges::checkPrivileges(this, TXT_ROOT_MSG_TITLE_WIN[lang], TXT_ROOT_MSG_WIN[lang]);

	/* turns page */
	global_w->setCurrentIndex(1);

	/* set up diagnostic + wiring some shit */
	Diagnostic *diag_obj = new Diagnostic;
	QThread *diag_thread = new QThread();
	diag_obj->moveToThread(diag_thread);

	/* thread start -> diagnostic start */
	QObject::connect(diag_thread, SIGNAL(started()), diag_obj, SLOT(onStarted()));
	/* update -> window */
	QObject::connect(diag_obj, SIGNAL(Update(int,QString)), this, SLOT(updateInfos(int,QString)));
	/* quit diagnostic -> quit thread */
	QObject::connect(diag_obj, SIGNAL(Quit()), diag_thread, SLOT(quit()));
	/* thread finished -> window's finish action */
	QObject::connect(diag_thread, SIGNAL(finished()), this, SLOT(finishAction()));
	diag_thread->start();

	/* set up timer */
	progress_timer = new QTimer(this);
	connect(progress_timer, SIGNAL(timeout()), this, SLOT(incrementTime()));
	progress_timer->setSingleShot(true);
	progress_timer->start();
}

void NetCheckerWindow::updateInfos(int progress_value, const QString info){
	progressbar_w->setValue(progress_value);
	log_w->append(info);
}

void NetCheckerWindow::finishAction(){
	/* if diagnostic is over, stop the timer and change text to finished */
	progress_timer->stop();
	if (!progress_info_time->isHidden()){
		progress_info_time->hide();
	}
	progress_info->setText(TXT_FINISHED[lang]);

	/* enable save buttons */
	clipboard_button_w->setEnabled(true);
	save_button_w->setEnabled(true);
}

void NetCheckerWindow::copyClipboard(){
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(log_w->toPlainText());
}

void NetCheckerWindow::saveToFile(){
	QString username = qgetenv("USER");
	if (username.isEmpty())
		username = qgetenv("USERNAME");
	QString filename = QFileDialog::getExistingDirectory(0, TXT_SAVE_DIALOG[lang]);
	filename.append(tr("/%1_%2.txt").arg(username).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_h-m-s")));
	if (!filename.isEmpty()){
		QFile file(filename);
		if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
			QTextStream stream(&file);
			stream.setCodec("UTF-8");
			stream << log_w->toPlainText();
			stream.flush();
			file.close();
		}
	}
}

void NetCheckerWindow::changeToHun(){
	if (lang != LANG::HUN){
		lang = LANG::HUN;
		initText();
	}
}

void NetCheckerWindow::changeToEng(){
	if (lang != LANG::ENG){
		lang = LANG::ENG;
		initText();
	}
}

void NetCheckerWindow::incrementTime(){
	static int count = 53;
	if (count >= 0){
		progress_info_time->setText(QString("%1s").arg(count--));
		progress_timer->start(1000);
	}
	else {
		progress_info->setText(TXT_ALMOST_END[lang]);
		if (!progress_info_time->isHidden()){
			progress_info_time->hide();
		}
	}
}
