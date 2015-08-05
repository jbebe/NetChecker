#ifndef DIALOG_H
#define DIALOG_H

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
    /* structure --> */
    QStackedWidget *global_w; QVBoxLayout *global_l;
        QWidget *welcome_page_w; QHBoxLayout *welcome_page_l;
            QWidget *logo_side_w; QVBoxLayout *logo_side_l;
                QLabel *logo_image_w;
            QWidget *eula_side_w; QVBoxLayout *eula_side_l;
                QTextEdit *text_w;
                QPushButton *next_button;
        QWidget *diagnostic_page_w; QVBoxLayout *diagnostic_page_l;
            QGroupBox *progressbar_holder_w; QVBoxLayout *progressbar_holder_l;
                QProgressBar *progressbar_w;
            QGroupBox *log_holder_w; QVBoxLayout *log_holder_l;
                QTextEdit *log_w;
            QWidget *button_holder_w; QHBoxLayout *button_holder_l;
                QPushButton* clipboard_button;
                QPushButton* save_button;
    /* <-- structure */
    int lang;
public:
    NetCheckerWindow();
public slots:
    void nextPage();
    void copyClipboard();
    void saveToFile();
    void changeToHun();
    void changeToEng();
private:
    void initWelcomePage();
    void initDiagnosticPage();
    void initText();
    enum {
        WINDOW_WIDTH = 350,
        WINDOW_HEIGHT = 400,
        LOGO_WIDTH = 120,
        EULA_WIDTH = (WINDOW_WIDTH-LOGO_WIDTH)
    };
};

#endif