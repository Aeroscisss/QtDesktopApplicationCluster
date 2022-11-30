#include "CustomFileMappingPatternWidget.h"
#include "GlobalMessageRepost.h"
#include "FileMappingManager.h"
CustomFileMappingPatternWidget::CustomFileMappingPatternWidget(FileMappingPattern& pattern,QWidget *parent)
	: CustomAbstractQWidget(parent),m_pattern(pattern)
{
	ui.setupUi(this);
	initWidget(m_pattern);
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
	for (auto iter = taskWidgetList.begin(); iter != taskWidgetList.end(); iter++) {
		(* iter)->deleteLater();
	}
	taskWidgetList.clear();
	//emplace new taskWidgetList
	auto taskList = pattern.getTaskList ();
	for (auto iter = taskList.begin(); iter != taskList.end(); ++iter) {
		CustomFileMappingTaskWidget* taskWidget = new CustomFileMappingTaskWidget(*iter, this);
		connect(taskWidget, &CustomFileMappingTaskWidget::sig_taskWidget_taskChanged,
			this, &CustomFileMappingPatternWidget::rec_updatePatternTask);
		taskWidgetList.append(taskWidget);
		ui.gridLayout_tasks->addWidget(taskWidget);
	}
	updateTaskAmountCounter();
}
void CustomFileMappingPatternWidget::updateTaskAmountCounter()
{
	ui.label_taskAmount->setText("(" + QString::number(taskWidgetList.size()) + ")");
}
void CustomFileMappingPatternWidget::rec_updatePatternTask()
{
	QList<FileMappingTask>taskList;
	for (auto iter : taskWidgetList) {
		taskList.append(iter->getTask());
	}
	m_pattern.updateTaskList(taskList);
	FileMappingManager::Instance().updatePattern(m_pattern.name(), m_pattern);
}
void CustomFileMappingPatternWidget::on_btn_addTask_clicked()
{
	CustomFileMappingTaskWidget* taskWidget = new CustomFileMappingTaskWidget(FileMappingTask(), this);
	connect(taskWidget, &CustomFileMappingTaskWidget::sig_taskWidget_taskChanged,
		this, &CustomFileMappingPatternWidget::rec_updatePatternTask); 
	taskWidget->setTaskMarkNum(taskWidgetList.size()+1);
	taskWidgetList.append(taskWidget);
	ui.gridLayout_tasks->addWidget(taskWidget);
}