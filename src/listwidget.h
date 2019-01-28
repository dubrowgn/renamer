#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QTableWidget>

class QMimeData;

class cListWidget : public QTableWidget
{
	Q_OBJECT

public:
	cListWidget(QWidget *parent = 0);
	~cListWidget();

public slots:
	void removeAll();
	void removeSelected();

private:
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *event);
	void dropEvent(QDropEvent *event);

	QRegExp *rxPath;
};

#endif // LISTWIDGET_H
