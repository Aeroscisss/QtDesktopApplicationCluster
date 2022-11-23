#pragma once
/*
	 ___________________
	/					\
	|	shared widget	|
	\__________________/
*/
#ifndef CUSTOM_MSG_BROWSER_WIDGET_H
#define CUSTOM_MSG_BROWSER_WIDGET_H

#include "CustomQObject/CustomAbstractQWidget.h"
#include "ui_CustomMsgBrowserWidget.h"

class CustomMsgBrowserWidget : public CustomAbstractQWidget
{
	Q_OBJECT

public:
	CustomMsgBrowserWidget(QWidget* parent = nullptr);
	~CustomMsgBrowserWidget();
	void setMsgTimePrifixAddition(bool addFix);
	void setCustomPrifixAddition(bool addCustomFix,QString text);
	void appendMsg(QString msg);
	void setLayoutDirection(bool isBottomUp);
public slots:
	void rec_appendMsg(QString msg);
private:
	Ui::CustomMsgBrowserWidgetClass ui;
	int defaultFontPointSize;
	bool addTimePrefix=true;
	bool addCustomPrefix=false;
	QString customPrifix;
private slots:
	void on_btn_clearTextBrowser_clicked();
	void on_btn_addFontSize_clicked();
	void on_btn_subFontSize_clicked();
	void on_btn_resetFontSize_clicked();
};

#endif // !CUSTOM_MSG_BROWSER_WIDGET_H
