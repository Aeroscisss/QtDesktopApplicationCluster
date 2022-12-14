#pragma once

#include <QWidget>
#include "CustomQObject\CustomAbstractQWidget.h"
#include "FileMappingTask.h"
#include "FileMappingRule.h"
#include "ui_CustomFileMappingTaskWidget.h"
#include "CustomQWidget/assembledQWidget/CustomQLabelEdit.h"

class CustomFileMappingTaskWidget : public CustomAbstractQWidget
{
	Q_OBJECT

public:
	explicit CustomFileMappingTaskWidget(FileMappingTask task,QWidget *parent = nullptr);
	~CustomFileMappingTaskWidget();
	FileMappingTask getTask();
	void setTaskMarkNum(int num);
signals:
	void sig_taskWidget_taskChanged();
	void sig_taskWidget_requestDelete();
private:
	Ui::CustomFileMappingTaskWidgetClass ui;
	CustomQLabelEdit* label_taskName;
private slots:
	void rec_updateTask();
	void on_btn_delTask_clicked();
};
