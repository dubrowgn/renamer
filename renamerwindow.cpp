#include <QtGui>

#include "renamerwindow.h"

cRenamerWindow::cRenamerWindow()
{
    lstFiles = new cListWidget;
    ckbRegex = new QCheckBox(tr("Use Regex"));
    ckbRegex->setChecked(true);
    layMain = new QGridLayout;
    layBtns = new QHBoxLayout;
    lblReplace = new QLabel(tr("Replace:"));
    lblSearch = new QLabel(tr("Search:"));
    lblStatus = new QLabel(tr("Drag and drop files/folders into the area above"
                              " to add them to your selection."));
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

    setLayout(layMain);
    setWindowTitle(tr("Multi-File Renamer - v1.3"));
    this->resize(600, 420);
}

void cRenamerWindow::replace()
{
    QRegExp rxReplace(txtSearch->text(), Qt::CaseSensitive, QRegExp::RegExp);
    if (!ckbRegex->checkState())
        rxReplace.setPatternSyntax(QRegExp::FixedString);
    else if (!rxReplace.isValid()) {
        lblStatus->setText(tr("There was an error in your regular expression"
                              " syntax. Please review it, and try again."));
        return;
    } // if / else/if
    QDir dir(tr("/"));
    QString path, name, name2;
    int i, size = lstFiles->rowCount(), errors = 0;
    for (i = 0; i < size; ++i) {
        name = name2 = lstFiles->item(i, 0)->text();
        if (rxReplace.indexIn(name) < 0)
            continue;
        path = lstFiles->item(i, 1)->text();
        name2.replace(rxReplace, txtReplace->text());
        if (name == name2)
            continue;
        dir.setPath(path);
        if (dir.rename(path + name, path + name2))
            lstFiles->item(i, 0)->setText(name2); // also check if flagged for errors?
        else {
            ++errors;
            // set error color/status
        } // if / else
    } // for
    if (errors > 0)
        lblStatus->setText(tr("There were unspecified errors durring the rename"
                              " process."));
    else
        lblStatus->setText(tr("Rename operation completed successfully."));
    lstFiles->resizeColumnsToContents();
    lstFiles->resizeRowsToContents();
    lstFiles->horizontalHeader()->setStretchLastSection(true);
}

void cRenamerWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Enter:
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
    event->accept();
}
