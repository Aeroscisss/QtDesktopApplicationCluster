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
	updateTimer->setInterval(300);
	//初始化widget
	initWidget();
}
CustomFileSyncPatternWidget::~CustomFileSyncPatternWidget()
{

}
void CustomFileSyncPatternWidget::initWidget()
{
	ui.label_patternName->setText(m_pattern.name().isEmpty()?"Pattern Name Empty": m_pattern.name());
	//std::lock_guard<std::mutex>locker(mutex_taskWidgets);
	for (auto iter = taskWidgetList.begin(); iter != taskWidgetList.end(); iter++) {
		(*iter)->deleteLater();
	}
	taskWidgetList.clear();
	auto taskList = m_pattern.getTaskList();
	int i = 0;
	for (auto iter = taskList.begin(); iter != taskList.end(); ++iter) {
		createNewTaskWidget(*iter, i + 1);
		i++;
	}
	updateTaskAmountCounter();
}
void CustomFileSyncPatternWidget::createNewTaskWidget(FileSyncTask task, int index)
{
	CustomFileSyncTaskWidget* taskWidget = new CustomFileSyncTaskWidget(task, index);
	connect(taskWidget, &CustomFileSyncTaskWidget::sig_taskWidget_taskChanged,
		this, &CustomFileSyncPatternWidget::rec_updateTask);
	connect(taskWidget, &CustomFileSyncTaskWidget::sig_taskWidget_requestDelete,
		this, &CustomFileSyncPatternWidget::rec_deleteTask);
	taskWidgetList.append(taskWidget);
	ui.gridLayout_tasks->addWidget(taskWidget);
	taskWidget->setTaskMarkNum(index);
}
void CustomFileSyncPatternWidget::updatePattern()
{
	QList<FileSyncTask>taskList;
	{
		//std::lock_guard<std::mutex>locker(mutex_taskWidgets);
		for (auto iter : taskWidgetList) {
			FileSyncTask task = iter->task;
			taskList.append(task);
		}
		m_pattern.updateTaskList(taskList);
	}
	FileSyncManager::Instance().updatePattern(m_pattern.name(), m_pattern);
}
void CustomFileSyncPatternWidget::updateTaskAmountCounter()
{
	ui.label_taskAmount->setText("(" + QString::number(taskWidgetList.size()) + ")");
}
void CustomFileSyncPatternWidget::rec_updateTask(){
	//接受到task已经更新信号，启动更新标志，开始计时器
	taskChanged = true;//task 更新标志
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
	createNewTaskWidget(FileSyncTask(), taskWidgetList.size()+1);
	updateTaskAmountCounter();
}