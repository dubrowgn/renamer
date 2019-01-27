#include <QtGui>

#include "listwidget.h"

cListWidget::cListWidget(QWidget *parent) : QTableWidget(parent)
{
#ifdef Q_OS_WIN32
    rxPath = new QRegExp(tr("^/(.*/)([^/]+)$"));
#else
    rxPath = new QRegExp(tr("^(.*/)([^/]+)$"));
#endif

    QStringList labels;
    labels << tr("Filename") << tr("Path");

    setAcceptDrops(true);
    setAlternatingRowColors(true);
    setColumnCount(2);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setHorizontalHeaderLabels(labels);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setShowGrid(false);
    setTabKeyNavigation(false);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    horizontalHeader()->setHighlightSections(false);
    horizontalHeader()->setStretchLastSection(true);
    resizeColumnsToContents();
    verticalHeader()->hide();
}

void cListWidget::removeAll()
{
    setRowCount(0);
    resizeColumnsToContents();
    resizeRowsToContents();
    horizontalHeader()->setStretchLastSection(true);
}

void cListWidget::removeSelected()
{
    int i, size = rowCount();
    for (i = 0; i < size; ++i) {
        if (item(i, 0)->isSelected()) {
            removeRow(i);
            --i;
            --size;
        } // if
    } // for
}

void cListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void cListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void cListWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void cListWidget::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        int row;
        for (int i = 0; i < urlList.size(); ++i) {
            if (rxPath->indexIn(urlList.at(i).path()) < 0)
                continue;
            row = rowCount();
            insertRow(row);
            setItem(row, 0, new QTableWidgetItem(rxPath->cap(2)));
            setItem(row, 1, new QTableWidgetItem(rxPath->cap(1)));
        } // for
    } // if
    resizeColumnsToContents();
    resizeRowsToContents();
    horizontalHeader()->setStretchLastSection(true);

    event->acceptProposedAction();
}
