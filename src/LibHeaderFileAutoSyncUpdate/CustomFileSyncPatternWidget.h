#pragma once

#include <QWidget>
#include <Qlist>
#include <QTimer>
#include "CustomQObject/CustomAbstractQWidget.h"
#include "FileSyncPattern.h"
#include "CustomFileSyncTaskWidget.h"
#include "ui_CustomFileSyncPatternWidget.h"

class CustomFileSyncPatternWidget : public CustomAbstractQWidget
{
	Q_OBJECT

public:
	explicit CustomFileSyncPatternWidget(FileSyncPattern &pattern,QWidget *parent = nullptr);
	~CustomFileSyncPatternWidget();
	void updatePattern();
private:
	void initWidget();
	void createNewTaskWidget(FileSyncTask task,int index);
	void updateTaskAmountCounter();
	Ui::CustomFileSyncPatternWidgetClass ui;
	FileSyncPattern m_pattern;

	std::mutex mutex_taskWidgets;
	QList<CustomFileSyncTaskWidget*>taskWidgetList;

	QTimer *updateTimer;
	bool taskChanged = false;
private slots:
	void on_btn_addTask_clicked();
	void rec_updateTask();
	void rec_deleteTask();
	void rec_timerForcedUpdatePattern();
};
