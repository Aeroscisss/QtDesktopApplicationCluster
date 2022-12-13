#include "CustomQMainWindow.h"
#include <QApplication>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <qlistview.h>
#include "GlobalMessageRepost.h"
#include "FileMappingManager.h"
#include "FileMappingRuleParser.h"
#include "FileMappingRule.h"
#include "FileMappingOperator.h"

CustomQMainWindow::CustomQMainWindow(QWidget *parent)
    : CustomAbstractQMainWindow(parent)
{
    ui.setupUi(this);
    qApp->setWindowIcon(QIcon(":/assets/icon/HeaderAsync128.ico"));
    QString style = QString("QComboBox QAbstractItemView { min-height: 20px; min-width: %1px}").arg(QString::number(300));
    ui.comboBox_paternSelect->setStyleSheet(style) ;
    ui.comboBox_paternSelect->setView(new QListView());///添加listview
    msgBrowser = new CustomMsgBrowserWidget(ui.frame_msgBrowser);
    msgBrowser->setDefaultPointSize(10);
    ui.gridLayout_msgBrowser->addWidget(msgBrowser); 
    stackedWidget_pattern = new QStackedWidget(this);
    ui.scrollArea_pattern->setWidget(stackedWidget_pattern);
    rec_refreshSaveAsAction();
    connectSigs();
}

CustomQMainWindow::~CustomQMainWindow()
{
    
}

void CustomQMainWindow::rec_refreshPatterns() {
    refreshPatternComboBox();
    refreshPatternContents();
}
void CustomQMainWindow::rec_refreshPatternContents()
{
    refreshPatternContents();
}
void CustomQMainWindow::rec_refreshSaveAsAction()
{
    if (FileMappingManager::Instance().getCurrentRuleFilePath().isEmpty()) {
        ui.action_saveFile->setVisible(false);
    }
    else {
        ui.action_saveFile->setVisible(true);
    }
}
void CustomQMainWindow::connectSigs()
{
    connect(&GlobalMessageRepost::Instance(), SIGNAL(sig_globalMessageRepost_mainWindowUserMsgBrowser(QString)),
        msgBrowser, SLOT(rec_appendMsg(QString)));
    connect(&FileMappingManager::Instance(), SIGNAL(sig_fileMappingManager_patternUpdated()),
        this, SLOT(rec_refreshPatterns()));
    connect(&FileMappingManager::Instance(), SIGNAL(sig_fileMappingManager_ruleFileOpened()),
        this, SLOT(rec_refreshSaveAsAction()));
	connect(ui.comboBox_paternSelect, SIGNAL(currentTextChanged(QString)),
		this, SLOT(rec_refreshPatternContents()));
    connect(this, SIGNAL(sig_requestOpenRuleFile(QString)),
        &FileMappingManager::Instance(), SLOT(rec_openRuleFile(QString)));
    connect(this, SIGNAL(sig_requestSaveRuleFile(QString)),
        &FileMappingManager::Instance(), SLOT(rec_saveRuleFile(QString)));
    connect(this, SIGNAL(sig_requestCreateNewPattern(QString)),
        &FileMappingManager::Instance(), SLOT(rec_createNewPattern(QString)));
    connect(this, SIGNAL(sig_requestDeletePattern(QString)),
        &FileMappingManager::Instance(), SLOT(rec_deletePattern(QString)));
    connect(this, SIGNAL(sig_requestPrintPatternsToConsole()),
        &FileMappingManager::Instance(), SLOT(rec_printPatternsToConsole()));
    
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
    if (currentPatternWidget != nullptr) {
        currentPatternWidget->updatePattern();//更新当前的Pattern
    }
    QString patternName = ui.comboBox_paternSelect->currentText();
    QString patternIndex = ui.comboBox_paternSelect->currentIndex();
    try {
        FileMappingPattern  pattern;
        if (!FileMappingManager::Instance().getPattern(patternName, pattern))
            throw std::exception("Can not Find Such Pattern. ");
        CustomFileMappingPatternWidget* patternWidget=nullptr;
        auto iter = map_patternWidget.find(patternName);
        if (iter==map_patternWidget.end()) {//不存在
            patternWidget= new CustomFileMappingPatternWidget(pattern, this);
            map_patternWidget.insert(patternName, patternWidget);
            stackedWidget_pattern->addWidget(patternWidget);
        }
        else {
            patternWidget = iter.value();
        }
        stackedWidget_pattern->setCurrentWidget(patternWidget);
        currentPatternWidget = patternWidget;
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
void CustomQMainWindow::on_action_printPatternsToConsole_triggered()
{
    emit sig_requestPrintPatternsToConsole();
}
void CustomQMainWindow::on_btn_apply_clicked()
{
    FileMappingOperator::Instance().threadLoopRun();
    if (currentPatternWidget != nullptr) {
        FileMappingPattern pattern;
        FileMappingManager::Instance().getPattern(ui.comboBox_paternSelect->currentText(), pattern);
        QList<FileMappingTask>tasks=pattern.getTaskList();
        for (auto task : tasks) {
            FileMappingRule rule=FileMappingRuleParser::Instance().parseRule(task.script());
            FileMappingOperator::Instance().ruleQueue->push(rule);
        }
    }
}
void CustomQMainWindow::on_action_openFile_triggered() 
{
    QString filePath=QFileDialog::getOpenFileName(this,
        tr(("Open Sync Rule File")),
        "",
		tr("Sync Rule File (*.syncrule)"));
	if (!filePath.isNull()){
		emit sig_requestOpenRuleFile(filePath);
	}
	else{
		//GlobalMessageRepost::Instance().sendNewMsg("Cancel Open Sync Rule File");
	}
}
void CustomQMainWindow::on_action_saveFile_triggered()
{
    QString filePath = FileMappingManager::Instance().getCurrentRuleFilePath();
    if (filePath.isEmpty()) {//未载入，调用零存在
        on_action_saveFileAs_triggered();
    }
    else {//载入了，存为原本文件
        //GlobalMessageRepost::Instance().sendNewMsg("Try Save Sync Rule File at \n[" + filePath + "]");
        emit sig_requestSaveRuleFile(filePath);
    }
}
void CustomQMainWindow::on_action_saveFileAs_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this,
        tr("Save File"),
        "",
        tr("Sync Rule File (*.syncrule)"));
    if (!filePath.isNull())
    {
        //GlobalMessageRepost::Instance().sendNewMsg("Try Save Sync Rule File at \n[" + filePath + "]");
        emit sig_requestSaveRuleFile(filePath);
    }
    else
    {
        GlobalMessageRepost::Instance().sendNewMsg("Cancel Save Sync Rule File");
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
