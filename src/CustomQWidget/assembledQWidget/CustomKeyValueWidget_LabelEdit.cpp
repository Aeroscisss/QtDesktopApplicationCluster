#include "CustomKeyValueWidget_LabelEdit.h"

CustomKeyValueWidget_LabelEdit::CustomKeyValueWidget_LabelEdit(QWidget* parent, bool editable,bool horizentalLayout)
	: CustomKeyValueWidget(parent,CustomKeyValueWidget::ValueWidgetType::LabelEdit,  editable,horizentalLayout)
{

	labelEdit_value = new CustomQLabelEdit(this);
	setEditable(editable);
	connect(labelEdit_value, SIGNAL(textChanged(QString)),
		this, SLOT(rec_textChanged(QString)));

	gridLayout_value->addWidget(labelEdit_value);

	currentValueWidget = labelEdit_value;
	
}

CustomKeyValueWidget_LabelEdit::~CustomKeyValueWidget_LabelEdit()
{
}

void CustomKeyValueWidget_LabelEdit::setStringValue(QString value)
{
	labelEdit_value->setText(value);
}

QString CustomKeyValueWidget_LabelEdit::getStringValue()
{
	return labelEdit_value->text();
}

void CustomKeyValueWidget_LabelEdit::setEditable(bool editable)
{
	labelEdit_value->setEditable(editable);
	CustomKeyValueWidget::setEditable(editable);
}

void CustomKeyValueWidget_LabelEdit::rec_textChanged(QString text) {
	emit sig_customKeyValueWidget_valueChanged();
	emit sig_customKeyValueWidget_labelEdit_textChanged(text);
}
