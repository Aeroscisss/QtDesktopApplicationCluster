#include "CustomKeyValueWidget_ComboBox.h"

CustomKeyValueWidget_ComboBox::CustomKeyValueWidget_ComboBox(QWidget* parent, bool editable, bool horizentalLayout)
	: CustomKeyValueWidget(parent, CustomKeyValueWidget::ValueWidgetType::ComboBox,editable,horizentalLayout)
{

	comboBox_value = new QComboBox(this);
	comboBox_value->setMinimumHeight(30);
	setEditable(editable);
	connect(comboBox_value, SIGNAL(currentIndexChanged(int)),
		this, SLOT(rec_valueChanged(int)));

	gridLayout_value->addWidget(comboBox_value);

	currentValueWidget = comboBox_value;

}

CustomKeyValueWidget_ComboBox::~CustomKeyValueWidget_ComboBox()
{
}

void CustomKeyValueWidget_ComboBox::setItemList(QList<QString> list)
{
	comboBox_value->clear();
	comboBox_value->addItems(list);
}

void CustomKeyValueWidget_ComboBox::setStringValue(QString string)
{
	comboBox_value->setCurrentText(string);
}

QString CustomKeyValueWidget_ComboBox::getStringValue()
{
	return comboBox_value->currentText();
}

void CustomKeyValueWidget_ComboBox::setEditable(bool editable)
{
	comboBox_value->setEnabled(editable);
	CustomKeyValueWidget::setEditable(editable);
}

void CustomKeyValueWidget_ComboBox::rec_valueChanged(int index) {
	emit sig_customKeyValueWidget_valueChanged();
	emit sig_customKeyValueWidget_comboBox_textChanged(comboBox_value->currentText());
}
