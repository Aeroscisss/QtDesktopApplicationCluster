#pragma once

#include <QWidget>
#include <Qlist>
#include <QTimer>
#include "CustomQObject/CustomAbstractQWidget.h"
#include "FileMappingPattern.h"
#include "CustomFileMappingTaskWidget.h"
#include "ui_CustomFileMappingPatternWidget.h"

class CustomFileMappingPatternWidget : public CustomAbstractQWidget
{
	Q_OBJECT

public:
	explicit CustomFileMappingPatternWidget(FileMappingPattern &pattern,QWidget *parent = nullptr);
	~CustomFileMappingPatternWidget();
	void updatePattern();
private:
	void initWidget(FileMappingPattern& pattern);
	void updateTaskAmountCounter();
	Ui::CustomFileMappingPatternWidgetClass ui;
	FileMappingPattern m_pattern;
	std::mutex mutex_taskWidgets;
	QList<CustomFileMappingTaskWidget*>taskWidgetList;
	QTimer *updateTimer;
	bool taskChanged = false;
private slots:
	void on_btn_addTask_clicked();
	void rec_updatePattern();
	void rec_deleteTask();
	void rec_timerForcedUpdatePattern();
};
