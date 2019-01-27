#ifndef RENAMERWINDOW_H
#define RENAMERWINDOW_H

#include <QWidget>
#include "listwidget.h"

class cListWidget;

class QCheckBox;
class QDialogButtonBox;
class QGridLayout;
class QHBoxLayout;
class QLabel;
class QMimeData;
class QPushButton;
class QTableWidget;

class cRenamerWindow : public QWidget
{
    Q_OBJECT

public:
    cRenamerWindow();

public slots:
    void replace();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    cListWidget *lstFiles;
    QCheckBox *ckbRegex;
    QGridLayout *layMain;
    QHBoxLayout *layBtns;
    QLabel *lblReplace;
    QLabel *lblSearch;
    QLabel *lblStatus;
    QLineEdit *txtReplace;
    QLineEdit *txtSearch;
    QPushButton *btnClear;
    QPushButton *btnReplace;
};

#endif // RENAMERWINDOW_H
