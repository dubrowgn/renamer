#include "list-widget.h"
#include "QDragLeaveEvent"
#include "QDir"
#include "QHeaderView"

ListWidget::ListWidget(QWidget *parent) : QTableWidget(parent)
{
    this->setFocusPolicy(Qt::StrongFocus);

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

ListWidget::~ListWidget() { }

void ListWidget::removeAll()
{
    setRowCount(0);
    resizeColumnsToContents();
    resizeRowsToContents();
    horizontalHeader()->setStretchLastSection(true);
}

void ListWidget::removeSelected()
{
    int i, size = rowCount();
    for (i = 0; i < size; ++i) {
        if (item(i, 0)->isSelected()) {
            removeRow(i);
            --i;
            --size;
        }
    }
}

void ListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void ListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void ListWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void ListWidget::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    if (!mimeData->hasUrls())
        return;

    for (QUrl url : mimeData->urls()) {
        if (url.scheme() != "file")
            continue;

        QFileInfo info(url.toLocalFile());
        int row = rowCount();

        insertRow(row);
        setItem(row, 0, new QTableWidgetItem(info.fileName()));
        setItem(row, 1, new QTableWidgetItem(info.path()));
    }

    resizeColumnsToContents();
    resizeRowsToContents();
    horizontalHeader()->setStretchLastSection(true);

    event->acceptProposedAction();
}
