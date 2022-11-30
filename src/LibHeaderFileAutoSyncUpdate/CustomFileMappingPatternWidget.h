#pragma once

#include <QWidget>
#include <Qlist>
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
private:
	void initWidget(FileMappingPattern& pattern);
	void updateTaskAmountCounter();
	Ui::CustomFileMappingPatternWidgetClass ui;
	FileMappingPattern m_pattern;
	QList<CustomFileMappingTaskWidget*>taskWidgetList;
private slots:
	void on_btn_addTask_clicked();
	void rec_updatePatternTask();
};
