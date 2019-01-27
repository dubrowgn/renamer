#ifndef RENAMERWINDOW_H
#define RENAMERWINDOW_H

#include <QWidget>
#include "listWidget.h"
//#include "logWindow.h"
#include "messageWidget.h"

class QCheckBox;
class QGridLayout;
class QHBoxLayout;
class QLabel;
class QPushButton;

class cRenamerWindow : public QWidget
{
	Q_OBJECT

public:
	cRenamerWindow();
	~cRenamerWindow();

public slots:
	void replace();

private:
	void keyPressEvent(QKeyEvent *_event);

	cListWidget *lstFiles;
	//cLogWindow *logWin;
	cMessageWidget *lblStatus;
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

#endif // RENAMERWINDOW_H
