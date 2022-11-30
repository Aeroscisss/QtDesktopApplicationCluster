#include "CustomKeyValueWidget_LcdNumberEdit.h"

CustomKeyValueWidget_LcdNumberEdit::CustomKeyValueWidget_LcdNumberEdit(QWidget* parent, bool editable, bool horizentalLayout)
	: CustomKeyValueWidget(parent,CustomKeyValueWidget::ValueWidgetType::LcdNumber, editable,horizentalLayout)
{
	lcdNumberEdit_value = new CustomQLcdNumberEdit(this);
	setEditable(editable);
	connect(lcdNumberEdit_value, SIGNAL(valueChanged(double)),
		this, SLOT(rec_valueChanged(double)));

	currentValueWidget = lcdNumberEdit_value;

	gridLayout_value->addWidget(lcdNumberEdit_value);
}

CustomKeyValueWidget_LcdNumberEdit::~CustomKeyValueWidget_LcdNumberEdit()
{
}

int CustomKeyValueWidget_LcdNumberEdit::getIntValue()
{
	return lcdNumberEdit_value->intValue();
}

double CustomKeyValueWidget_LcdNumberEdit::getValue()
{
	return lcdNumberEdit_value->value();
}

void CustomKeyValueWidget_LcdNumberEdit::setValue(double value)
{
	lcdNumberEdit_value->display(value);
}

void CustomKeyValueWidget_LcdNumberEdit::setStringValue(QString value)
{
	lcdNumberEdit_value->display(value.toDouble());
}

QString CustomKeyValueWidget_LcdNumberEdit::getStringValue()
{
	return QString::number(lcdNumberEdit_value->value());
}

void CustomKeyValueWidget_LcdNumberEdit::setEditable(bool editable)
{
	lcdNumberEdit_value->setEnabled(editable);
	CustomKeyValueWidget::setEditable(editable);
}

CustomQLcdNumberEdit* CustomKeyValueWidget_LcdNumberEdit::getCustomQLcdNumberEditPtr()
{
	return lcdNumberEdit_value;
}

void CustomKeyValueWidget_LcdNumberEdit::rec_valueChanged(double value) {
	emit sig_customKeyValueWidget_valueChanged();
	emit sig_customKeyValueWidget_lcdNumberEdit_valueChanged(value);
}
