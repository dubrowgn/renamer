#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QTableWidget>

class QMimeData;

class cListWidget : public QTableWidget
{
    Q_OBJECT

public:
    cListWidget(QWidget *parent = 0);

public slots:
    void removeAll();
    void removeSelected();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

private:    
    QRegExp *rxPath;
};

#endif // LISTWIDGET_H
