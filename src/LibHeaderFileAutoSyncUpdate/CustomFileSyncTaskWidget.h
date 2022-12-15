#pragma once

#include <QWidget>
#include "CustomQObject\CustomAbstractQWidget.h"
#include "FileSyncTask.h"
#include "FileSyncRule.h"
#include "ui_CustomFileSyncTaskWidget.h"
#include "CustomQWidget/assembledQWidget/CustomQLabelEdit.h"

class CustomFileSyncTaskWidget : public CustomAbstractQWidget
{
	Q_OBJECT

public:
	explicit CustomFileSyncTaskWidget(FileSyncTask task,int taskNum,QWidget *parent = nullptr);
	~CustomFileSyncTaskWidget();
	FileSyncTask getTask();
	void setTaskMarkNum(int num);
	int taskMarkNum();
signals:
	void sig_taskWidget_taskChanged();
	void sig_taskWidget_requestDelete();
private:
	Ui::CustomFileSyncTaskWidgetClass ui;
	CustomQLabelEdit* label_taskName;
	int m_taskNum;
private slots:
	void rec_updateTask();
	void on_btn_delTask_clicked();
};
