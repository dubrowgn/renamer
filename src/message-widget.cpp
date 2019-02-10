#include "message-widget.h"

MessageWidget::MessageWidget(const QString & defaultMessage, int defaultDurationMs)
:	QLabel(defaultMessage),
    defaultMessage(defaultMessage),
    defaultDurationMs(defaultDurationMs)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showDefaultMessage()));
}

MessageWidget::~MessageWidget() {
    delete timer;
}

void MessageWidget::showMessage(const QString &msg) {
    setText(msg);
    timer->start(defaultDurationMs);
}

void MessageWidget::showMessage(const QString &msg, int durationMs) {
    setText(msg);
    timer->start(durationMs);
}

void MessageWidget::showDefaultMessage() {
    setText(defaultMessage);
}
