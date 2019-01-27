#include "messageWidget.h"

cMessageWidget::cMessageWidget(const QString & _defaultMessage, int _defaultMessageTime)
:	QLabel(_defaultMessage),
	defaultMessage(_defaultMessage),
	defaultMessageTime(_defaultMessageTime)
{
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(showDefaultMessage()));
} // cMessageWidget( QString, int )

cMessageWidget::~cMessageWidget() {
	delete timer;
} // ~cMessageWidget( void )

void cMessageWidget::showMessage(const QString & _msg, int _msgTime) {
	setText(_msg);

	// can't have a negative timer duration
	if (_msgTime < 0)
		timer->start(defaultMessageTime);
	else
		timer->start(_msgTime);
} // showMessage( QString, int )

void cMessageWidget::showDefaultMessage() {
	setText(defaultMessage);
} // showDefaultMessage( void )
