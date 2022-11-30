#include "CustomKeyValueWidget_LineEdit.h"

CustomKeyValueWidget_LineEdit::CustomKeyValueWidget_LineEdit(QWidget* parent, bool editable,bool horizentalLayout)
	: CustomKeyValueWidget(parent, CustomKeyValueWidget::ValueWidgetType::LineEdit, editable,horizentalLayout)
{
	lineEdit_value = new QLineEdit(this);
	setEditable(editable);
	connect(lineEdit_value, SIGNAL(textChanged(QString)),
		this, SLOT(rec_textChanged(QString)));

	currentValueWidget = lineEdit_value;

	gridLayout_value->addWidget(lineEdit_value);
}

CustomKeyValueWidget_LineEdit::~CustomKeyValueWidget_LineEdit()
{
}

void CustomKeyValueWidget_LineEdit::setStringValue(QString value)
{
	lineEdit_value->setText(value);
}

QString CustomKeyValueWidget_LineEdit::getStringValue()
{
	return lineEdit_value->text();
}

void CustomKeyValueWidget_LineEdit::setEditable(bool editable)
{
	lineEdit_value->setEnabled (editable);
	CustomKeyValueWidget::setEditable(editable);
}

void CustomKeyValueWidget_LineEdit::rec_textChanged(QString value) {
	emit sig_customKeyValueWidget_valueChanged();
	emit sig_customKeyValueWidget_lineEdit_textChanged(value);
}
