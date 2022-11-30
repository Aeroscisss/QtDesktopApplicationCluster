#include "CustomKeyValueWidget_CheckBox.h"

CustomKeyValueWidget_CheckBox::CustomKeyValueWidget_CheckBox(QWidget* parent ,bool editable,bool triState, bool horizentalLayout)
	: CustomKeyValueWidget(parent, CustomKeyValueWidget::ValueWidgetType::CheckBox,editable,horizentalLayout)
{
	checkBox_value = new QCheckBox(this);
	checkBox_value->setTristate(triState);
	checkBox_value->setText("");
	setEditable(editable);
	connect(checkBox_value, SIGNAL(stateChanged(int)),
		this, SLOT(rec_checkStateChanged(int)));
	
	currentValueWidget = checkBox_value;

	gridLayout_value->addWidget(checkBox_value);
}

CustomKeyValueWidget_CheckBox::~CustomKeyValueWidget_CheckBox()
{
}

void CustomKeyValueWidget_CheckBox::setValue(int state)
{
	if (checkBox_value->isTristate()) {
		if (state == 0 || state == 1 || state == 2) {
			checkBox_value->setCheckState((Qt::CheckState)state);
		}
		return;
	}
	else {
		if (state >= 0) {
			checkBox_value->setChecked((bool)state);
		}
		return;
	}
}

int CustomKeyValueWidget_CheckBox::getValue()
{
	if(checkBox_value->isTristate())
		return (int)checkBox_value->checkState();
	return (int)checkBox_value->isChecked();
}

void CustomKeyValueWidget_CheckBox::setStringValue(QString value)
{
	if (checkBox_value->isTristate()) {
		if (value.toLower() == QString("Checked").toLower() || value.toLower() == QString("true").toLower())
			checkBox_value->setCheckState(Qt::CheckState::Checked);
		if (value.toLower() == QString("PartiallyChecked").toLower() || value.toLower() == QString("Partially Checked").toLower())
			checkBox_value->setCheckState(Qt::CheckState::PartiallyChecked);
		if (value.toLower() == QString("UnChecked").toLower() || value.toLower() == QString("false").toLower())
			checkBox_value->setCheckState(Qt::CheckState::Unchecked);
	}
	else {
		if (value.toLower() == QString("Checked").toLower() || value.toLower() == QString("true").toLower())
			checkBox_value->setChecked(true);
		if (value.toLower() == QString("UnChecked").toLower() || value.toLower() == QString("false").toLower())
			checkBox_value->setChecked(false);
	}
}

QString CustomKeyValueWidget_CheckBox::getStringValue()
{
	switch (checkBox_value->checkState())
	{
	case Qt::CheckState::Unchecked:
		return "Unchecked";
	case Qt::CheckState::PartiallyChecked:
		return "PartiallyChecked";
	case Qt::CheckState::Checked:
		return "Checked";
	default:
		return "";
	}
}

void CustomKeyValueWidget_CheckBox::setEditable(bool editable)
{
	checkBox_value->setEnabled(editable);
	CustomKeyValueWidget::setEditable(editable);
}

//void CustomKeyValueWidget_CheckBox::setIndicatorSize(QSize size)
//{
//	QString styleS = "QCheckBox::indicator {\nwidth: "
//		+ QString::number(size.width()) + "px; height: "
//		+ QString::number(size.height()) + "px ;};";
//	checkBox_value->setStyleSheet(styleS);
//	checkBox_value->setIconSize(size);
//}

void CustomKeyValueWidget_CheckBox::rec_checkStateChanged(int state){
	emit sig_customKeyValueWidget_valueChanged();
	emit sig_customKeyValueWidget_checkBox_checkStateChanged(state);
}
