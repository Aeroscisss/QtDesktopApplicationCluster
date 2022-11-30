#include "CustomKeyValueWidget_TextEdit.h"

CustomKeyValueWidget_TextEdit::CustomKeyValueWidget_TextEdit(QWidget* parent, bool editable, bool horizentalLayout)
	: CustomKeyValueWidget(parent, CustomKeyValueWidget::ValueWidgetType::TextEdit,  editable,horizentalLayout)
{

	textEdit_value = new QTextEdit(this);
	setEditable(editable);
	connect(textEdit_value, SIGNAL(textChanged()),
		this, SLOT(rec_textChanged()));

	gridLayout_value->addWidget(textEdit_value);

	currentValueWidget = textEdit_value;

}

CustomKeyValueWidget_TextEdit::~CustomKeyValueWidget_TextEdit()
{
}

void CustomKeyValueWidget_TextEdit::setStringValue(QString value)
{
	textEdit_value->setText(value);
}

QString CustomKeyValueWidget_TextEdit::getStringValue()
{
	return textEdit_value->toPlainText();
}

void CustomKeyValueWidget_TextEdit::setEditable(bool editable)
{
	textEdit_value->setEnabled(editable);
	CustomKeyValueWidget::setEditable(editable);
}

void CustomKeyValueWidget_TextEdit::rec_textChanged() {
	emit sig_customKeyValueWidget_valueChanged();
	emit sig_customKeyValueWidget_textEdit_textChanged(textEdit_value->toPlainText());
}
