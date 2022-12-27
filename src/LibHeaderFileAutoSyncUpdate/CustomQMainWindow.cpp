#include "CustomQMainWindow.h"
#include <QApplication>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <qlistview.h>
#include "GlobalMessageRepost.h"
#include "FileSyncManager.h"
#include "FileSyncRuleParser.h"
#include "FileSyncRule.h"
#include "FileSyncOperator.h"

CustomQMainWindow::CustomQMainWindow(QWidget *parent)
	: CustomAbstractQMainWindow(parent)
{
	ui.setupUi(this);
	qApp->setWindowIcon(QIcon(":/assets/icon/HeaderAsync128.ico"));
	QString style = QString("QComboBox QAbstractItemView { min-height: 20px; min-width: %1px}").arg(QString::number(300));
	ui.comboBox_paternSelect->setStyleSheet(style) ;
	ui.comboBox_paternSelect->setView(new QListView());///添加listview
	//添加控制台
	msgBrowser = new CustomMsgBrowserWidget(ui.frame_msgBrowser);
	msgBrowser->setDefaultPointSize(10);
	ui.gridLayout_msgBrowser->addWidget(msgBrowser); 
	//添加stack
	stackedWidget_pattern = new QStackedWidget(this);
	ui.scrollArea_pattern->setWidget(stackedWidget_pattern);
	this->connectSigs();
}

CustomQMainWindow::~CustomQMainWindow()
{
	
}
void CustomQMainWindow::rec_resetPatterns() {
	rec_updateTitle();
	refreshPatternComboBox();
	for (auto item : map_patternWidget) {
		item->deleteLater();
	}
	map_patternWidget.clear();
	currentPatternWidget = nullptr;
}

void CustomQMainWindow::rec_refreshPatterns() {
	//QString str = static_cast<QString>(this->sender()->objectName());
	//GlobalMessageRepost::Instance().sendNewMsg("refreshPatterns sender "+ str, 1);
	refreshPatternComboBox();
	refreshPatternContents();
}
void CustomQMainWindow::rec_refreshPatternContents()
{
	refreshPatternContents();
}
void CustomQMainWindow::rec_refreshSaveAsAction()
{
	if (FileSyncManager::Instance().getCurrentRuleFilePath().isEmpty()) {
		ui.action_saveFileAs->setShortcut(QCoreApplication::translate("CustomQMainWindowClass", "Ctrl+S", nullptr));
		ui.action_saveFile->setShortcut(QKeySequence());
	}
	else {
		ui.action_saveFileAs->setShortcut(QKeySequence());
		ui.action_saveFile->setShortcut(QCoreApplication::translate("CustomQMainWindowClass", "Ctrl+S", nullptr));
	}
}
void CustomQMainWindow::rec_updateTitle()
{
	QString path=FileSyncManager::Instance().getCurrentRuleFilePath();
	QString title = "FileSync";
	if (!path.isEmpty()) {
		QFileInfo info(path);
		if (info.exists()) {
			title += " -------- [" + info.completeBaseName() +"."+ info.suffix() + "]";
		}
	}
	this->setWindowTitle(title);
}
void CustomQMainWindow::connectSigs()
{
	connect(&GlobalMessageRepost::Instance(), SIGNAL(sig_globalMessageRepost_mainWindowUserMsgBrowser(QString)),
		msgBrowser, SLOT(rec_appendMsg(QString)));
	
	connect(&FileSyncManager::Instance(), SIGNAL(sig_fileSyncManager_ruleFileOpened()),
		this, SLOT(rec_refreshSaveAsAction()));
	connect(&FileSyncManager::Instance(), SIGNAL(sig_fileSyncManager_ruleFileOpened()),
		this, SLOT(rec_updateTitle()));
	connect(&FileSyncManager::Instance(), SIGNAL(sig_fileSyncManager_ruleFileOpened()),
		this, SLOT(rec_refreshPatterns()));
	connect(&FileSyncManager::Instance(), SIGNAL(sig_fileSyncManager_patternUpdated()),
		this, SLOT(rec_refreshPatterns()));
	connect(&FileSyncManager::Instance(), SIGNAL(sig_fileSyncManager_ruleFileClosed()),
		this, SLOT(rec_resetPatterns()));

	connect(ui.comboBox_paternSelect, SIGNAL(currentTextChanged(QString)),
		this, SLOT(rec_refreshPatternContents()));
	connect(this, SIGNAL(sig_requestOpenRuleFile(QString)),
		&FileSyncManager::Instance(), SLOT(rec_openRuleFile(QString)));
	connect(this,&CustomQMainWindow::sig_requestCloseCurrRuleFile,
		&FileSyncManager::Instance(), &FileSyncManager::rec_closeCurrRuleFile);
	connect(this, SIGNAL(sig_requestSaveRuleFile(QString)),
		&FileSyncManager::Instance(), SLOT(rec_saveRuleFile(QString)));
	connect(this, SIGNAL(sig_requestCreateNewPattern(QString)),
		&FileSyncManager::Instance(), SLOT(rec_createNewPattern(QString)));
	connect(this, SIGNAL(sig_requestDeletePattern(QString)),
		&FileSyncManager::Instance(), SLOT(rec_deletePattern(QString)));
	connect(this, SIGNAL(sig_requestPrintPatternsToConsole()),
		&FileSyncManager::Instance(), SLOT(rec_printPatternsToConsole()));

	GlobalMessageRepost::Instance().sendNewMsg("CustomQMainWindow::Signal Connected", 1);
	
}
void CustomQMainWindow::refreshPatternComboBox()
{
	ui.comboBox_paternSelect->blockSignals(true);
	ui.comboBox_paternSelect->clear();
	QStringList names=FileSyncManager::Instance().getPatternNames();
	ui.comboBox_paternSelect->addItems(names);
	ui.comboBox_paternSelect->setCurrentIndex(names.size()-1);
	ui.comboBox_paternSelect->blockSignals(false);
}
void CustomQMainWindow::refreshPatternContents()
{
	GlobalMessageRepost::Instance().sendNewMsg("refreshPatternContents", 1 );
	QString patternName = ui.comboBox_paternSelect->currentText();
	QString patternIndex = ui.comboBox_paternSelect->currentIndex();
	try {
		FileSyncPattern  pattern;
		if (!FileSyncManager::Instance().getPattern(patternName, pattern))//获取当前patternName
			throw std::exception("Can not Find Such Pattern. ");
		auto iter = map_patternWidget.find(patternName);
		if (iter==map_patternWidget.end()) {//不存在
			currentPatternWidget = new CustomFileSyncPatternWidget(pattern, this);
			currentPatternWidget->setObjectName(patternName);
			map_patternWidget.insert(patternName, currentPatternWidget);
			stackedWidget_pattern->addWidget(currentPatternWidget);
		}
		else {
			currentPatternWidget = iter.value();
		}
		stackedWidget_pattern->setCurrentWidget(currentPatternWidget);
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
	if (result == QMessageBox::Yes){
		map_patternWidget.remove(currPatternName);
		if (currentPatternWidget != nullptr) {
			currentPatternWidget->deleteLater();
			currentPatternWidget = nullptr;
		}
		emit sig_requestDeletePattern(currPatternName);
	}
}
void CustomQMainWindow::on_action_printPatternsToConsole_triggered()
{
	emit sig_requestPrintPatternsToConsole();
}
void CustomQMainWindow::on_btn_apply_clicked()
{
	FileSyncOperator::Instance().threadLoopRun();
	if (currentPatternWidget != nullptr) {
		FileSyncPattern pattern;
		FileSyncManager::Instance().getPattern(ui.comboBox_paternSelect->currentText(), pattern);
		QList<FileSyncTask>tasks=pattern.getTaskList();
		for (auto task : tasks) {
			FileSyncRule rule=FileSyncRuleParser::Instance().parseRule(task.script());
			FileSyncOperator::Instance().ruleQueue->push(rule);
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
	QString filePath = FileSyncManager::Instance().getCurrentRuleFilePath();
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
void CustomQMainWindow::on_action_closeFile_triggered()
{
	emit sig_requestCloseCurrRuleFile();
}
void CustomQMainWindow::on_action_close_triggered()
{
	qApp->quit();
}
void CustomQMainWindow::on_action_createNewPattern_triggered() {
	bool ok=false;
	//输入名字
	QString string = QInputDialog::getText(this,
		tr("Input"),
		tr("Input pattern name:"),
		QLineEdit::Normal,
		FileSyncManager::Instance().newPatternNameSuggestion(),
		&ok
	);
	if (ok) {
		emit sig_requestCreateNewPattern(string);
	}
	else {
		GlobalMessageRepost::Instance().sendNewMsg("Cancel create new pattern."); 
	}
}
