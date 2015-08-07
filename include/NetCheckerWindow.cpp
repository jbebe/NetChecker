#include "NetCheckerWindow.hpp"
#include "Diagnostic.hpp"
#include "Translation.hpp"

NetCheckerWindow::NetCheckerWindow(){
    /* default language is hungarian */
    LANG = HUN;
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
    global_l->setMargin(0);
    global_l->setSpacing(0);

    initWelcomePage();
    initDiagnosticPage();

    setLayout(global_l);
}

void NetCheckerWindow::initWelcomePage(){
    /* inicializálás */
    welcome_page_l = new QHBoxLayout;
    welcome_page_w = new QWidget;
    welcome_page_l->setMargin(0);
    welcome_page_l->setSpacing(0);

    initLogoSide();
    initReadmeSide();

    welcome_page_w->setLayout(welcome_page_l);
    global_w->addWidget(welcome_page_w);
}

void NetCheckerWindow::initLogoSide(){
    logo_side_w = new QWidget;
    logo_side_l = new QVBoxLayout;

    logo_side_w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    /* logo image at the left */
    logo_image_w = new QLabel;
    QPixmap pix_logo(":/rsrc/logo.png");
    pix_logo = pix_logo.scaledToWidth(LOGO_WIDTH, Qt::SmoothTransformation);
    logo_image_w->setPixmap(pix_logo);
    logo_image_w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    /* flags for choosing language */
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
    flag_eng_w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    flag_eng_w->setFixedSize(icon_size.width()+4, icon_size.height()+4);
    flag_eng_w->setIcon(icon_eng);

    flag_hun_w->setIconSize(icon_size);
    flag_hun_w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    flag_hun_w->setFixedSize(icon_size.width()+4, icon_size.height()+4);
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

    text_w = new QTextEdit;
    text_w->setReadOnly(true);

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

    progressbar_holder_w->setLayout(progressbar_holder_l);
    progressbar_holder_l->addWidget(progressbar_w);
    progressbar_holder_w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    /* log */
    log_holder_w = new QGroupBox;
    log_holder_l = new QVBoxLayout;
    log_w = new QTextEdit;
    log_w->setReadOnly(true);
    //log_w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    log_holder_w->setLayout(log_holder_l);
    log_holder_l->addWidget(log_w);

    /* buttons */
    button_holder_w = new QWidget;
    button_holder_l = new QHBoxLayout;
    clipboard_button_w = new QPushButton;
    save_button_w = new QPushButton;
    clipboard_button_w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    save_button_w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(clipboard_button_w, SIGNAL(released()), this, SLOT(copyClipboard()));
    connect(save_button_w, SIGNAL(released()), this, SLOT(saveToFile()));

    button_holder_l->addWidget(clipboard_button_w);
    button_holder_l->addWidget(save_button_w);
    button_holder_w->setLayout(button_holder_l);
    button_holder_w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    /* add to parent */
    diagnostic_page_l->addWidget(progressbar_holder_w);
    diagnostic_page_l->addWidget(log_holder_w);
    diagnostic_page_l->addWidget(button_holder_w, 0, Qt::AlignRight);
    diagnostic_page_w->setLayout(diagnostic_page_l);

    global_w->addWidget(diagnostic_page_w);
}

void NetCheckerWindow::initText(){
    /* set program title */
    setWindowTitle(TXT_APP_TITLE[LANG]);

    next_button_w->setText(TXT_NEXT_BTN[LANG]);

    QFile readme_file(README_URL[LANG]);
    readme_file.open(QIODevice::ReadOnly);
    QTextStream readme_file_str(&readme_file);
    readme_file_str.setCodec("UTF-8");
    text_w->setHtml(readme_file_str.readAll());
    readme_file.close();

    progressbar_holder_w->setTitle(TXT_DIAG_GROUP[LANG]);
    log_holder_w->setTitle(TXT_LOG_GROUP[LANG]);
    clipboard_button_w->setText(TXT_CLIPB_BTN[LANG]);
    save_button_w->setText(TXT_SAVE_BTN[LANG]);
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
    QString filename = QFileDialog::getExistingDirectory(0, TXT_SAVE_DIALOG[LANG]);
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
    if (LANG != HUN){
        LANG = HUN;
        initText();
    }
}

void NetCheckerWindow::changeToEng(){
    if (LANG != ENG){
        LANG = ENG;
        initText();
    }
}
