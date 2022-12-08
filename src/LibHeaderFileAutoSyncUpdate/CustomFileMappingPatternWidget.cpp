#include "CustomFileMappingPatternWidget.h"
#include "GlobalMessageRepost.h"
#include "FileMappingManager.h"
CustomFileMappingPatternWidget::CustomFileMappingPatternWidget(FileMappingPattern& pattern,QWidget *parent)
	: CustomAbstractQWidget(parent),m_pattern(pattern)
{
	ui.setupUi(this);
	initWidget(m_pattern);
	updateTimer = new QTimer(this);
	connect(updateTimer, &QTimer::timeout, this, &CustomFileMappingPatternWidget::rec_timerForcedUpdatePattern);
	updateTimer->setInterval(1000);
}

CustomFileMappingPatternWidget::~CustomFileMappingPatternWidget()
{

}
void CustomFileMappingPatternWidget::initWidget(FileMappingPattern& pattern)
{
	//设置patternName显示
	if (pattern.name().isEmpty()) {
		ui.label_patternName->setText("Pattern Name Empty");
	}
	else {
		ui.label_patternName->setText(pattern.name());
	}
	//清空taskWidgetList
	std::lock_guard<std::mutex>locker(mutex_taskWidgets);
	for (auto iter = taskWidgetList.begin(); iter != taskWidgetList.end(); iter++) {
		(*iter)->deleteLater();
	}
	taskWidgetList.clear();
	//emplace new taskWidgetList
	auto taskList = pattern.getTaskList ();
	int i = 0;
	for (auto iter = taskList.begin(); iter != taskList.end(); ++iter) {
		CustomFileMappingTaskWidget* taskWidget = new CustomFileMappingTaskWidget(*iter, this);
		connect(taskWidget, &CustomFileMappingTaskWidget::sig_taskWidget_taskChanged,
			this, &CustomFileMappingPatternWidget::rec_updatePattern);
		taskWidgetList.append(taskWidget);
		ui.gridLayout_tasks->addWidget(taskWidget);
		taskWidget->setTaskMarkNum(i + 1);
		i++;
	}
	updateTaskAmountCounter();
}
void CustomFileMappingPatternWidget::updatePattern()
{
	QList<FileMappingTask>taskList;
	std::lock_guard<std::mutex>locker(mutex_taskWidgets);
	for (auto iter : taskWidgetList) {
		FileMappingTask task = iter->getTask();
		taskList.append(task);
	}
	m_pattern.updateTaskList(taskList);
	FileMappingManager::Instance().updatePattern(m_pattern.name(), m_pattern);
}
void CustomFileMappingPatternWidget::updateTaskAmountCounter()
{
	ui.label_taskAmount->setText("(" + QString::number(taskWidgetList.size()) + ")");
}
void CustomFileMappingPatternWidget::rec_updatePattern()
{
	taskChanged = true;//task更新标志
	updateTimer->start();//开始计时
}
void CustomFileMappingPatternWidget::rec_timerForcedUpdatePattern()
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
void CustomFileMappingPatternWidget::on_btn_addTask_clicked()
{
	std::lock_guard<std::mutex>locker(mutex_taskWidgets);
	CustomFileMappingTaskWidget* taskWidget = new CustomFileMappingTaskWidget(FileMappingTask(), this);
	connect(taskWidget, &CustomFileMappingTaskWidget::sig_taskWidget_taskChanged,
		this, &CustomFileMappingPatternWidget::rec_updatePattern); 
	taskWidget->setTaskMarkNum(taskWidgetList.size()+1);
	taskWidgetList.append(taskWidget);
	ui.gridLayout_tasks->addWidget(taskWidget);
	updateTaskAmountCounter();
}