#include "CustomFileSyncPatternWidget.h"
#include "GlobalMessageRepost.h"
#include "FileSyncManager.h"
CustomFileSyncPatternWidget::CustomFileSyncPatternWidget(FileSyncPattern& pattern,QWidget *parent)
	: CustomAbstractQWidget(parent),m_pattern(pattern)
{
	ui.setupUi(this);
	//创建计时器
	updateTimer = new QTimer(this);
	connect(updateTimer, &QTimer::timeout, this, &CustomFileSyncPatternWidget::rec_timerForcedUpdatePattern);
	updateTimer->setInterval(1000);
	//初始化widget
	initWidget();
}
CustomFileSyncPatternWidget::~CustomFileSyncPatternWidget()
{

}
void CustomFileSyncPatternWidget::initWidget()
{
	//GlobalMessageRepost::Instance().sendNewMsg("debug", 1);
	//QTime time;
	//time.start();
	//设置patternName显示
	ui.label_patternName->setText(m_pattern.name().isEmpty()?"Pattern Name Empty": m_pattern.name());
	//qDebug() << time.elapsed() << endl;
	//清空taskWidgetList
	std::lock_guard<std::mutex>locker(mutex_taskWidgets);
	for (auto iter = taskWidgetList.begin(); iter != taskWidgetList.end(); iter++) {
		(*iter)->deleteLater();
	}
	taskWidgetList.clear();
	//qDebug() << time.elapsed() << endl;
	//emplace new taskWidgetList
	auto taskList = m_pattern.getTaskList();
	int i = 0;
	for (auto iter = taskList.begin(); iter != taskList.end(); ++iter) {
		CustomFileSyncTaskWidget* taskWidget = new CustomFileSyncTaskWidget(*iter,i);
		connect(taskWidget, &CustomFileSyncTaskWidget::sig_taskWidget_taskChanged,
			this, &CustomFileSyncPatternWidget::rec_updateTask);
		connect(taskWidget, &CustomFileSyncTaskWidget::sig_taskWidget_requestDelete,
			this, &CustomFileSyncPatternWidget::rec_deleteTask);
		taskWidgetList.append(taskWidget);
		ui.gridLayout_tasks->addWidget(taskWidget);
		taskWidget->setTaskMarkNum(i + 1);
		i++;
	}
	updateTaskAmountCounter();
	//qDebug() << time.elapsed() << endl;
}
void CustomFileSyncPatternWidget::updatePattern()
{
	QList<FileSyncTask>taskList;
	std::lock_guard<std::mutex>locker(mutex_taskWidgets);
	for (auto iter : taskWidgetList) {
		FileSyncTask task = iter->getTask();
		taskList.append(task);
	}
	m_pattern.updateTaskList(taskList);
	FileSyncManager::Instance().updatePattern(m_pattern.name(), m_pattern);
}
void CustomFileSyncPatternWidget::updateTaskAmountCounter()
{
	ui.label_taskAmount->setText("(" + QString::number(taskWidgetList.size()) + ")");
}
void CustomFileSyncPatternWidget::rec_updateTask(){
	//接受到task已经更新信号，启动更新标志，开始计时器
	taskChanged = true;//task更新标志
	updateTimer->start();//开始计时
}
void CustomFileSyncPatternWidget::rec_deleteTask()
{
	CustomFileSyncTaskWidget* taskWidget = reinterpret_cast<CustomFileSyncTaskWidget*> (sender());
	if (taskWidgetList.removeOne(taskWidget)) {
		GlobalMessageRepost::Instance().sendNewMsg("Task removed", 1);
		m_pattern.removeTask(taskWidget->taskMarkNum());
		taskWidget->deleteLater();
	}
	else {
		GlobalMessageRepost::Instance().sendNewMsg("Fail removing Task", 1);
	}
}
void CustomFileSyncPatternWidget::rec_timerForcedUpdatePattern()
{
	if (taskChanged) {
		updatePattern();
		taskChanged = false;
		GlobalMessageRepost::Instance().sendNewMsg("Timer Force to update pattern",1);
	}
	else {
		//GlobalMessageRepost::Instance().sendNewMsg("Timer time out, but no change", 1);
	}
}
void CustomFileSyncPatternWidget::on_btn_addTask_clicked()
{
	m_pattern.addTask(FileSyncTask());
	initWidget();
}