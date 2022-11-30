#include "CustomSettingPageWidget.h"
#include "customQWidget/assembledQWidget/CustomKeyValueWidget_CheckBox.h"
#include "customQWidget/assembledQWidget/CustomKeyValueWidget_ComboBox.h"
#include "customQWidget/assembledQWidget/CustomKeyValueWidget_LabelEdit.h"
#include "customQWidget/assembledQWidget/CustomKeyValueWidget_LcdNumberEdit.h"
#include "customQWidget/assembledQWidget/CustomKeyValueWidget_LineEdit.h"
#include "customQWidget/assembledQWidget/CustomKeyValueWidget_PathSelectButton.h"
#include "customQWidget/assembledQWidget/CustomKeyValueWidget_TextEdit.h"
#include <QGridLayout>
#include "PiStandaloneReport.h"
using namespace Pi;
using namespace std;
CustomSettingPageWidget::CustomSettingPageWidget(QWidget *parent)
	: QWidget(parent)
{
	gridLayout_main = new QGridLayout(this);
}
CustomSettingPageWidget::~CustomSettingPageWidget()
{
}
void CustomSettingPageWidget::addSettingWidget(Pi::PiAbstractSettingItem<bool>* settingItem, CustomKeyValueWidget::ValueWidgetType type) {
	try {
		if (settingItem == nullptr)
			throw std::exception("SettingItem Ptr is Empty addSettingWidget bool settingItem fail");
		switch (type)
		{
		case CustomKeyValueWidget::ValueWidgetType::CheckBox: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::ComboBox:{
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::LabelEdit: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::LcdNumber: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::LineEdit: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::PathSelectButton: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::TextEdit: {
			break;
		}
		}
	}
	catch (std::exception e) {
		Pi::PiStandaloneReport::debugReport("CustomSettingPageWidget", e.what());
	}
}
void CustomSettingPageWidget::addSettingWidget(Pi::PiAbstractSettingItem<std::string>* settingItem, CustomKeyValueWidget::ValueWidgetType type) {
	try {
		if (settingItem == nullptr)
			throw std::exception("SettingItem Ptr is Empty addSettingWidget std::string settingItem fail");
		switch (type)
		{
		case CustomKeyValueWidget::ValueWidgetType::CheckBox: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::ComboBox: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::LabelEdit: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::LcdNumber: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::LineEdit: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::PathSelectButton: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::TextEdit: {
			break;
		}
		}
	}
	catch (std::exception e) {
		Pi::PiStandaloneReport::debugReport("CustomSettingPageWidget", e.what());
	}
}
void CustomSettingPageWidget::addSettingWidget(Pi::PiAbstractSettingItem<int>* settingItem, CustomKeyValueWidget::ValueWidgetType type) {
	try {
		if (settingItem == nullptr)
			throw std::exception("SettingItem Ptr is Empty addSettingWidget std::string settingItem fail");
		switch (type)
		{
		case CustomKeyValueWidget::ValueWidgetType::CheckBox: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::ComboBox: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::LabelEdit: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::LcdNumber: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::LineEdit: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::PathSelectButton: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::TextEdit: {
			break;
		}
		}
	}
	catch (std::exception e) {
		Pi::PiStandaloneReport::debugReport("CustomSettingPageWidget", e.what());
	}
}
void CustomSettingPageWidget::addSettingWidget(Pi::PiAbstractSettingItem<float>* settingItem, CustomKeyValueWidget::ValueWidgetType type) {
	try {
		if (settingItem == nullptr)
			throw std::exception();
		switch (type)
		{
		case CustomKeyValueWidget::ValueWidgetType::CheckBox: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::ComboBox: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::LabelEdit: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::LcdNumber: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::LineEdit: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::PathSelectButton: {
			break;
		}
		case CustomKeyValueWidget::ValueWidgetType::TextEdit: {
			break;
		}
		}
	}
	catch (std::exception e) {
		Pi::PiStandaloneReport::debugReport("CustomSettingPageWidget", e.what());
	}
}