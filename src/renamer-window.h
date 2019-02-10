#ifndef RENAMER_WINDOW_H
#define RENAMER_WINDOW_H

#include "list-widget.h"
#include "message-widget.h"
#include "QCheckBox"
#include "QGridLayout"
#include "QHBoxLayout"
#include "QLabel"
#include "QMainWindow"
#include "QPushButton"

class RenamerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RenamerWindow(QWidget *parent = nullptr);
    ~RenamerWindow();

public slots:
    void replace();

private:
    void keyPressEvent(QKeyEvent *_event);

    QWidget *centralWidget;
    ListWidget *lstFiles;
    MessageWidget *lblStatus;
    QCheckBox *ckbRegex;
    QGridLayout *layMain;
    QHBoxLayout *layBtns;
    QLabel *lblReplace;
    QLabel *lblSearch;
    QLineEdit *txtReplace;
    QLineEdit *txtSearch;
    QPushButton *btnClear;
    QPushButton *btnReplace;
};

#endif // RENAMER_WINDOW_H
