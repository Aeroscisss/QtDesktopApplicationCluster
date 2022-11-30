#include "CustomKeyValueWidget_PathSelectButton.h"

CustomKeyValueWidget_PathSelectButton::CustomKeyValueWidget_PathSelectButton(QWidget* parent, bool editable, bool horizentalLayout)
	: CustomKeyValueWidget(parent, CustomKeyValueWidget::ValueWidgetType::PathSelectButton,editable,horizentalLayout)
{

	pathSelectButton_value = new CustomQPathSelectButton(this);
	pathSelectButton_value->setPathOnBtnDispArg(true,false);
	setEditable(editable);
	connect(pathSelectButton_value, SIGNAL(sig_customPathSelectButton_pathChanged(QList<QString>)),
		this, SLOT(rec_pathChanged(QList<QString>)));

	gridLayout_value->addWidget(pathSelectButton_value);

	currentValueWidget = pathSelectButton_value;

}

CustomKeyValueWidget_PathSelectButton::~CustomKeyValueWidget_PathSelectButton()
{
}

void CustomKeyValueWidget_PathSelectButton::setStringValue(QString value)
{
	pathSelectButton_value->setText(value);
}

QString CustomKeyValueWidget_PathSelectButton::getStringValue()
{
	if (!pathSelectButton_value->getPaths().empty())
		return pathSelectButton_value->getReletivePaths()[0];
	return "";
}

void CustomKeyValueWidget_PathSelectButton::setPaths(QList<QString> paths)
{
	pathSelectButton_value->setUncertainPaths(paths);
}

QStringList CustomKeyValueWidget_PathSelectButton::getPaths()
{
	return pathSelectButton_value->getPaths();
}

void CustomKeyValueWidget_PathSelectButton::setEditable(bool editable)
{
	pathSelectButton_value->setEditable(editable);
	CustomKeyValueWidget::setEditable(editable);
}

void CustomKeyValueWidget_PathSelectButton::rec_pathChanged(QList<QString> paths) {
	emit sig_customKeyValueWidget_valueChanged();
	emit sig_customKeyValueWidget_pathSelectButton_pathChanged(paths);
}
