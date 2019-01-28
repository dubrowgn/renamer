#include <QtGui>
#include "logWindow.h"

cLogWindow::cLogWindow(QWidget* _parent) : QWidget(_parent) {
	lstLog = new QTableWidget;
	layBtns = new QHBoxLayout;
	layMain = new QGridLayout;
	btnClear = new QPushButton(tr("Clear Log"));
	btnClose = new QPushButton(tr("Close"));

	connect(btnClear, SIGNAL(pressed()), this, SLOT(removeAll()));
	connect(btnClose, SIGNAL(pressed()), this, SLOT(hide()));

	layBtns->addWidget(btnClear);
	layBtns->addStretch();
	layBtns->addWidget(btnClose);

	layMain->addWidget(lstLog, 0, 0, 1, 3);
	layMain->addLayout(layBtns, 1, 0, 1, 3);

	setLayout(layMain);
	setWindowTitle(tr("Renamer - Error Log"));
	this->resize(420, 360);
} // cLogWindow( QWidget* )

cLogWindow::~cLogWindow() {
	// clean-up widgets
	delete lstLog;
	delete btnClear;
	delete btnClose;

	// clean-up layouts
	delete layBtns;
	delete layMain;
}

void cLogWindow::removeAll()
{
	lstLog->setRowCount(0);
}

void cLogWindow::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
		case Qt::Key_Escape:
			hide();
			break;
		default:
			return;
	} // switch
	event->accept();
}
