#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QLabel>
#include <QTimer>

//class QMimeData;

class cMessageWidget : public QLabel
{
	Q_OBJECT

public:
	cMessageWidget(const QString & _defaultMessage, int _defaultMessageTime = 5000);
	~cMessageWidget();

	void showMessage(const QString & _msg, int _msgTime = -1);

private slots:
	void showDefaultMessage();

private:
	QString defaultMessage;
	int defaultMessageTime;
	QTimer* timer;
};

#endif // MESSAGEWIDGET_H
