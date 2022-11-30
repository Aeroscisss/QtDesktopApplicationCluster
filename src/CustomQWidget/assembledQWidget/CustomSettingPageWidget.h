#pragma once

#include <list>
#include <string>
#include <QWidget>
#include "customQWidget/assembledQWidget/CustomKeyValueWidget.h"
#include "PiGlobalSetting/PiAbstractSettingItem.h"
class CustomSettingPageWidget : public QWidget
{
	Q_OBJECT

public:
	CustomSettingPageWidget(QWidget *parent = nullptr);
	~CustomSettingPageWidget();
	void addSettingWidget(Pi::PiAbstractSettingItem<bool>*settingItem,CustomKeyValueWidget::ValueWidgetType type);
	void addSettingWidget(Pi::PiAbstractSettingItem<std::string>* settingItem, CustomKeyValueWidget::ValueWidgetType type);
	void addSettingWidget(Pi::PiAbstractSettingItem<int>* settingItem, CustomKeyValueWidget::ValueWidgetType type);
	void addSettingWidget(Pi::PiAbstractSettingItem<float>* settingItem, CustomKeyValueWidget::ValueWidgetType type);
private:
	QGridLayout* gridLayout_main = nullptr;
	std::list<CustomKeyValueWidget*>valueWidgetList;//���ṩ���ʣ��������ͷţ��ͷ�����Qt Parent Tree;
};
