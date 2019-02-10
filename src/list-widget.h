#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include "QMimeData"
#include "QTableWidget"

class ListWidget : public QTableWidget
{
    Q_OBJECT

public:
    ListWidget(QWidget *parent = nullptr);
    ~ListWidget();

public slots:
    void removeAll();
    void removeSelected();

private:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // LISTWIDGET_H
