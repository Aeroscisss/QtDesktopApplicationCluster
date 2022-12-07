#include "CustomQMainWindow.h"
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include "GlobalMessageRepost.h"
#include "FileMappingManager.h"
CustomQMainWindow::CustomQMainWindow(QWidget *parent)
    : CustomAbstractQMainWindow(parent)
{
    ui.setupUi(this);
    qApp->setWindowIcon(QIcon(":/assets/icon/HeaderAsync128.ico"));
    msgBrowser = new CustomMsgBrowserWidget(ui.frame_msgBrowser);
    msgBrowser->setDefaultPointSize(10);
    ui.gridLayout_msgBrowser->addWidget(msgBrowser); 
    
    connectSigs();
}

CustomQMainWindow::~CustomQMainWindow()
{
    
}

void CustomQMainWindow::rec_refreshPatterns() {
    refreshPatternComboBox();
    refreshPatternContents();
}
void CustomQMainWindow::rec_refresPatternContents()
{
    refreshPatternContents();
}
void CustomQMainWindow::connectSigs()
{
    connect(&GlobalMessageRepost::Instance(), SIGNAL(sig_globalMessageRepost_mainWindowUserMsgBrowser(QString)),
        msgBrowser, SLOT(rec_appendMsg(QString)));
    connect(&FileMappingManager::Instance(), SIGNAL(sig_fileMappingManager_patternUpdated()),
        this, SLOT(rec_refreshPatterns()));
    connect(this, SIGNAL(sig_requestCreateNewPattern(QString)),
        &FileMappingManager::Instance(), SLOT(rec_createNewPattern(QString)));
    connect(this, SIGNAL(sig_requestDeletePattern(QString)),
        &FileMappingManager::Instance(), SLOT(rec_deletePattern(QString)));
    connect(ui.comboBox_paternSelect, SIGNAL(currentTextChanged(QString)),
        this, SLOT(rec_refresPatternContents()));
}
void CustomQMainWindow::refreshPatternComboBox()
{
    ui.comboBox_paternSelect->blockSignals(true);
    ui.comboBox_paternSelect->clear();
    QStringList names=FileMappingManager::Instance().getPatternNames();
    ui.comboBox_paternSelect->addItems(names);
    ui.comboBox_paternSelect->setCurrentIndex(names.size()-1);
    ui.comboBox_paternSelect->blockSignals(false);
}
void CustomQMainWindow::refreshPatternContents()
{
    QString patternName = ui.comboBox_paternSelect->currentText();
    try {
        FileMappingPattern  pattern;
        if (!FileMappingManager::Instance().getPattern(patternName, pattern))
            throw std::exception("Can not Find Such Pattern. ");
        patternWidget = std::make_unique<CustomFileMappingPatternWidget>(pattern);
        ui.verticalLayout_pattern->addWidget(patternWidget.get());
    }
    catch (std::exception e) {
        QString errMsg = QString::fromStdString(e.what())+"Pattern Name:["+patternName+"]";
        GlobalMessageRepost::Instance().sendNewMsg(errMsg);
    }
  
}
void CustomQMainWindow::on_action_delCurrPattern_triggered()
{
    QString currPatternName = ui.comboBox_paternSelect->currentText();
    QString msg = tr("Delete Current Pattern [") + currPatternName + "] ?";
    int result =
        QMessageBox::question(this, tr("Delete?"), msg, QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    if (result == QMessageBox::Yes)
    {
        emit sig_requestDeletePattern(currPatternName);
    }
}
void CustomQMainWindow::on_action_createNewPattern_triggered() {
	bool ok=false;
	//输入名字
	QString string = QInputDialog::getText(this, tr("Input"), tr("Input pattern name:"), QLineEdit::Normal, FileMappingManager::Instance().newPatternNameSuggestion(), &ok, Qt::WindowCloseButtonHint);
    if (ok) {
        emit sig_requestCreateNewPattern(string);
    }
    else {
        GlobalMessageRepost::Instance().sendNewMsg("Cancel create new pattern."); 
    }
}
