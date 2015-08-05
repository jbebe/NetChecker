#include "NetCheckerWindow.hpp"
#include "Diagnostic.hpp"
#include "Translation.hpp"

NetCheckerWindow::NetCheckerWindow()
: lang(HUN)
{
    setLayout(global_l = new QVBoxLayout);

    global_l->addWidget(global_w = new QStackedWidget);
    global_w->addWidget(welcome_page_w = new QWidget);
    global_w->addWidget(diagnostic_page_w = new QWidget);

    initWelcomePage();
    initDiagnosticPage();
    initText();

    setWindowTitle(tr("NetChecker"));
}

void NetCheckerWindow::initWelcomePage(){
    /* inicializálás */
    welcome_page_l = new QHBoxLayout;
    logo_side_w = new QWidget;
    logo_side_l = new QVBoxLayout;
    logo_image_w = new QLabel;
    eula_side_w = new QWidget;
    eula_side_l = new QVBoxLayout;
    text_w = new QTextEdit;
    next_button = new QPushButton;

    welcome_page_w->setLayout(welcome_page_l);

    /* logo tulajdonságok */
    logo_image_w->setAlignment(Qt::AlignCenter);
    logo_image_w->setMinimumSize(LOGO_WIDTH, WINDOW_HEIGHT);
    QPixmap pix_logo(":/rsrc/logo.png");
    pix_logo = pix_logo.scaledToWidth(LOGO_WIDTH, Qt::SmoothTransformation);
    logo_image_w->setPixmap(pix_logo);

    /* zászlók */
    QWidget *flag_holder_w = new QWidget;
    QHBoxLayout *flag_holder_l = new QHBoxLayout;
    QPushButton *flag_eng_w = new QPushButton;
    QPushButton *flag_hun_w = new QPushButton;
    QPixmap pix_flag_eng(":/rsrc/flag_eng.gif");
    QPixmap pix_flag_hun(":/rsrc/flag_hun.gif");
    QIcon icon_eng(pix_flag_eng);
    QIcon icon_hun(pix_flag_hun);
    QSize icon_size(pix_flag_eng.width(), pix_flag_eng.height());
    flag_eng_w->setIconSize(icon_size);
    flag_hun_w->setIconSize(icon_size);
    flag_eng_w->setIcon(icon_eng);
    flag_hun_w->setIcon(icon_hun);
    connect(flag_eng_w, SIGNAL(released()), this, SLOT(changeToEng()));
    connect(flag_hun_w, SIGNAL(released()), this, SLOT(changeToHun()));

    flag_holder_l->addWidget(flag_hun_w);
    flag_holder_l->addWidget(flag_eng_w);
    flag_holder_w->setLayout(flag_holder_l);

    /* logo és zászló hozzáadása az ablakhoz */
    logo_side_l->addWidget(logo_image_w);
    logo_side_l->addWidget(flag_holder_w);
    logo_side_w->setLayout(logo_side_l);

    /* eula szöveg hozzáadása */
    text_w->setMinimumSize(EULA_WIDTH, WINDOW_HEIGHT);
    text_w->setReadOnly(true);

    /* gomb a továbblépéshez */

    next_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(next_button, SIGNAL(released()), this, SLOT(nextPage()));

    eula_side_l->addWidget(text_w);
    eula_side_l->addWidget(next_button);
    eula_side_w->setLayout(eula_side_l);

    welcome_page_l->addWidget(logo_side_w);
    welcome_page_l->addWidget(eula_side_w);
    welcome_page_w->setLayout(welcome_page_l);
}

void NetCheckerWindow::initDiagnosticPage(){
    /* diagnostic oldal felépítése */
    diagnostic_page_l = new QVBoxLayout;
    progressbar_holder_w = new QGroupBox;
    progressbar_holder_l = new QVBoxLayout;
    progressbar_w = new QProgressBar;
    log_holder_w = new QGroupBox;
    log_holder_l = new QVBoxLayout;
    log_w = new QTextEdit;
    button_holder_w = new QWidget;
    button_holder_l = new QHBoxLayout;
    clipboard_button = new QPushButton;
    save_button = new QPushButton;

    progressbar_w->setRange(0, 100);
    progressbar_w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    diagnostic_page_l->addWidget(progressbar_holder_w);
    progressbar_holder_w->setLayout(progressbar_holder_l);
    progressbar_holder_l->addWidget(progressbar_w);
    progressbar_w->setAlignment(Qt::AlignHCenter);
    progressbar_holder_w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    /* log */
    log_w->setReadOnly(true);
    log_w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    log_holder_w->setLayout(log_holder_l);
    log_holder_l->addWidget(log_w);

    /* buttons */
    clipboard_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    save_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(clipboard_button, SIGNAL(released()), this, SLOT(copyClipboard()));
    connect(save_button, SIGNAL(released()), this, SLOT(saveToFile()));

    button_holder_l->addWidget(clipboard_button);
    button_holder_l->addWidget(save_button);
    button_holder_w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    button_holder_w->setLayout(button_holder_l);

    /* add everything */
    diagnostic_page_l->addWidget(progressbar_holder_w);
    diagnostic_page_l->addWidget(log_holder_w);
    diagnostic_page_l->addWidget(button_holder_w);
    diagnostic_page_w->setLayout(diagnostic_page_l);
}

void NetCheckerWindow::initText(){

    next_button->setText(TXT_NEXT_BTN[lang]);

    /* eula szöveg betöltése fájlból */
    QFile eula_file(README_URL[lang]);
    eula_file.open(QIODevice::ReadOnly);
    QTextStream readme_file_str(&eula_file);
    readme_file_str.setCodec("UTF-8");
    text_w->setHtml(readme_file_str.readAll());
    eula_file.close();

    progressbar_holder_w->setTitle(TXT_DIAG_GROUP[lang]);
    log_holder_w->setTitle(TXT_LOG_GROUP[lang]);
    clipboard_button->setText(TXT_CLIPB_BTN[lang]);
    save_button->setText(TXT_SAVE_BTN[lang]);
}

void NetCheckerWindow::nextPage(){
    global_w->setCurrentIndex(1);
    log_w->append(Diagnostic::getInterfaceInfo());
    progressbar_w->setValue(50);
    log_w->append(Diagnostic::getRoutingInfo());
    progressbar_w->setValue(100);
}

void NetCheckerWindow::copyClipboard(){
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(log_w->toPlainText());
}

void NetCheckerWindow::saveToFile(){
    QString filename = QFileDialog::getExistingDirectory(0, TXT_SAVE_DIALOG[lang]);
    filename.append(tr("/log.txt"));
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
    if (lang != HUN){
        lang = HUN;
        initText();
    }
}

void NetCheckerWindow::changeToEng(){
    if (lang != ENG){
        lang = ENG;
        initText();
    }
}
