#include "renamer-window.h"
#include "QDir"
#include "QHeaderView"
#include "QKeyEvent"
#include "QLineEdit"
#include "QRegularExpression"
#include "QTextStream"

RenamerWindow::RenamerWindow(QWidget *parent) :
    QMainWindow(parent)
{
    lstFiles = new ListWidget;
    ckbRegex = new QCheckBox(tr("Use Regex"));
    ckbRegex->setChecked(true);
    layMain = new QGridLayout;
    layBtns = new QHBoxLayout;
    lblReplace = new QLabel(tr("Replace:"));
    lblSearch = new QLabel(tr("Search:"));
    lblStatus = new MessageWidget(tr("Drag and drop files/folders into the area "
                                     "above to add them to your selection."));
    lblStatus->setWordWrap(true);
    lblStatus->setAlignment(Qt::AlignHCenter);
    txtReplace = new QLineEdit;
    txtReplace->setAcceptDrops(false);
    txtSearch = new QLineEdit;
    txtSearch->setAcceptDrops(false);
    btnClear = new QPushButton(tr("Clear List"));
    btnReplace = new QPushButton(tr("Search and Replace"));

    connect(btnClear, SIGNAL(pressed()), lstFiles, SLOT(removeAll()));
    connect(btnReplace, SIGNAL(pressed()), this, SLOT(replace()));

    layBtns->addWidget(ckbRegex);
    layBtns->addStretch();
    layBtns->addWidget(btnClear);
    layBtns->addWidget(btnReplace);

    layMain->addWidget(lstFiles, 0, 0, 1, 2);
    layMain->addWidget(lblStatus, 1, 0, 1, 2);
    layMain->addWidget(lblSearch);
    layMain->addWidget(txtSearch);
    layMain->addWidget(lblReplace);
    layMain->addWidget(txtReplace);
    layMain->addLayout(layBtns, 4, 0, 1, 2);

    centralWidget = new QWidget();
    centralWidget->setLayout(layMain);
    setCentralWidget(centralWidget);
    setWindowTitle(tr("Multi-File Renamer - v1.4.0"));
    resize(600, 420);
}

RenamerWindow::~RenamerWindow() {
    delete btnClear;
    delete btnReplace;
    delete ckbRegex;
    delete lblReplace;
    delete lblSearch;
    delete lblStatus;
    delete lstFiles;
    delete txtReplace;
    delete txtSearch;

    delete layBtns;
    delete layMain;

    delete centralWidget;
}

void RenamerWindow::replace() {
    QString pattern = txtSearch->text();
    if (!ckbRegex->checkState())
        pattern = QRegularExpression::escape(pattern);

    QRegularExpression rxReplace(pattern);
    if (!rxReplace.isValid()) {
        lblStatus->setText(rxReplace.errorString());
        return;
    }

    QDir dir(tr("/"));
    QString path, newName, oldName;
    int i, size = lstFiles->rowCount(), errors = 0;
    for (i = 0; i < size; ++i) {
        newName = oldName = lstFiles->item(i, 0)->text();
        path = lstFiles->item(i, 1)->text();

        newName.replace(rxReplace, txtReplace->text());
        if (newName == oldName)
            continue;

        dir.setPath(path);
        if (dir.rename(oldName, newName))
            lstFiles->item(i, 0)->setText(newName);
        else
            ++errors;
    } // for

    if (errors > 0) {
        QString msg;
        QTextStream(&msg) << "There were " << errors << "errors durring the rename process";
        lblStatus->showMessage(msg);
    } else {
        lblStatus->showMessage(tr("Rename operation completed successfully."));
    } // if/else

    lstFiles->resizeColumnsToContents();
    lstFiles->resizeRowsToContents();
    lstFiles->horizontalHeader()->setStretchLastSection(true);
}

void RenamerWindow::keyPressEvent(QKeyEvent *_event) {
    switch (_event->key()) {
        case Qt::Key_Enter: // enter key on number pad
        case Qt::Key_Return: // main enter/return key
            replace();
            break;
        case Qt::Key_Delete:
            lstFiles->removeSelected();
            break;
        case Qt::Key_Escape:
            close();
            break;
        default:
            return;
    } // switch
    _event->accept();
}
