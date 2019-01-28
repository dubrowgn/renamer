#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QTableWidget>

class QGridLayout;
class QHBoxLayout;
class QPushButton;
class QTableWidget;

class cLogWindow : public QWidget
{
	Q_OBJECT

public:
	cLogWindow(QWidget *parent = 0);
	~cLogWindow();

	// error( const QString )
	// detachList()
	// attachList()
	// setMaxErrors( unsigned int/long )

public slots:
	void removeAll();

private:
	void keyPressEvent(QKeyEvent *event);

	QTableWidget *lstLog;
	QGridLayout *layMain;
	QHBoxLayout *layBtns;
	QPushButton *btnClear;
	QPushButton *btnClose;
};

#endif // LOGWIDGET_H
