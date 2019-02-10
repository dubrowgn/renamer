#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include "QLabel"
#include "QString"
#include "QTimer"

class MessageWidget : public QLabel
{
    Q_OBJECT

public:
    MessageWidget(const QString &defaultMessage, int defaultDurationMs = 5000);
    ~MessageWidget();

    void showMessage(const QString & msg);
    void showMessage(const QString & msg, int durationMs);

private slots:
    void showDefaultMessage();

private:
    QString defaultMessage;
    int defaultDurationMs;
    QTimer* timer;
};

#endif // MESSAGEWIDGET_H
