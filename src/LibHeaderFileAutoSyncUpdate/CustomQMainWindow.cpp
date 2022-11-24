#include "CustomQMainWindow.h"
#include <QApplication>
#include <QInputDialog>
#include "GlobalMessageRepost.h"
CustomQMainWindow::CustomQMainWindow(QWidget *parent)
    : CustomAbstractQMainWindow(parent)
{
    ui.setupUi(this);
    qApp->setWindowIcon(QIcon(":/assets/icon/HeaderAsync128.ico"));
    msgBrowser = new CustomMsgBrowserWidget(ui.frame_msgBrowser);
    ui.gridLayout_msgBrowser->addWidget(msgBrowser);
    
}

CustomQMainWindow::~CustomQMainWindow()
{
}

void CustomQMainWindow::connectSigs()
{
    connect(&GlobalMessageRepost::Instance(), SIGNAL(sig_globalMessageRepost_mainWindowUserMsgBrowser(QString)),
        msgBrowser, SLOT(rec_appendMsg(QString)));
}
void CustomQMainWindow::on_action_createNewPattern_triggered() {
	bool ok=false;
	// »ñÈ¡×Ö·û´®
	QString string = QInputDialog::getText(this, tr("Input pattern name"), tr("Input pattern:"), QLineEdit::Normal, tr("New Pattern"), &ok);
    if (ok) {
        QString msg = QString("New Pattern Name:[")  + string + QString("]");
        GlobalMessageRepost::Instance().sendNewMsg(msg, GlobalMessageRepost::MsgDst::MainWindowUserMsgBrowser);
    }
}
