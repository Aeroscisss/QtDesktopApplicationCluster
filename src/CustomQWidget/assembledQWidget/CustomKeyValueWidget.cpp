#include "CustomKeyValueWidget.h"
#include <QApplication>
CustomKeyValueWidget::CustomKeyValueWidget(QWidget* parent , ValueWidgetType in_widgetType, bool in_editable, bool horizentalLayout)
	: QWidget(parent),widgetType(in_widgetType),editable(in_editable),m_horizentalLayout(horizentalLayout)
{
	//主体
	resize(874, 659);
	setMinimumSize(QSize(0, 30));
	setMaximumSize(QSize(16777215, 16777215));
	gridLayout = new QGridLayout(this);
	gridLayout->setSpacing(6);
	gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
	gridLayout->setContentsMargins(0, 0, 0, 0);
	//主框体
	frame_main = new QFrame(this);
	frame_main->setObjectName(QString::fromUtf8("frame_main"));
	frame_main->setStyleSheet(QString::fromUtf8(""));
	frame_main->setFrameShape(QFrame::NoFrame);
	frame_main->setFrameShadow(QFrame::Plain);
	frame_main->setLineWidth(2);
	gridLayout_main = new QGridLayout(frame_main);
	gridLayout_main->setSpacing(3);
	gridLayout_main->setObjectName(QString::fromUtf8("gridLayout_main"));
	gridLayout_main->setContentsMargins(1, 1, 1, 1);
	//key
	label_key = new QLabel(frame_main);
	label_key->setMinimumHeight(30);
	label_key->setObjectName(QString::fromUtf8("label_key"));
	label_key->setAlignment(Qt::AlignCenter);
	//value
	frame_value = new QFrame(frame_main);
	frame_value->setObjectName(QString::fromUtf8("frame_value"));
	frame_value->setMinimumHeight(30);
	frame_value->setFrameShape(QFrame::NoFrame);
	frame_value->setFrameShadow(QFrame::Plain);
	gridLayout_value = new QGridLayout(frame_value);
	gridLayout_value->setSpacing(6);
	gridLayout_value->setObjectName(QString::fromUtf8("gridLayout_value"));
	gridLayout_value->setContentsMargins(0, 0, 0, 0);
	if (m_horizentalLayout) {
		gridLayout_main->addWidget(label_key, 0, 0, 1, 1);
		gridLayout_main->addWidget(frame_value, 0, 1, 1, 1);
	}
	else {
		gridLayout_main->addWidget(label_key, 0, 0, 1, 1);
		gridLayout_main->addWidget(frame_value, 1, 0, 1, 1);
	}

	gridLayout->addWidget(frame_main, 0, 0, 1, 1);

	highLightColor = Qt::red;
}

CustomKeyValueWidget::~CustomKeyValueWidget()
{
}

CustomKeyValueWidget::ValueWidgetType CustomKeyValueWidget::getWidgetType()
{
	return widgetType;
}
void CustomKeyValueWidget::setValueWidgetMinHeight(int height){
	this->setMinimumHeight(height);
	label_key->setMinimumHeight(height);
	if(currentValueWidget!=nullptr)
		currentValueWidget->setMinimumHeight(height);
}
void CustomKeyValueWidget::setValueWidgetMinWidth(int width)
{
	this->setMinimumWidth(width);
}
void CustomKeyValueWidget::setKey(QString key)
{
	label_key->setText(key);
}

QString CustomKeyValueWidget::getKey()
{
	return label_key->text();
}

void CustomKeyValueWidget::setKeyVisible(bool visibility)
{
	if (visibility) {
		label_key->setVisible(true);
	}
	else {
		label_key->setVisible(false);
	}
}

void CustomKeyValueWidget::setStringValue(QString in_value)
{
}

QString CustomKeyValueWidget::getStringValue()
{
	return QString("CustomKeyValueWidget::no String Value in CustomKeyValueWidget");
}

void CustomKeyValueWidget::setEditable(bool in_editable)
{
	editable = in_editable;
}

bool CustomKeyValueWidget::getEditable()
{
	return editable;
}

bool CustomKeyValueWidget::setKeyValueExpandRatio(float ratio)
{
	if (ratio < 0)
		return false;
	QSizePolicy keyP = label_key->sizePolicy();
	QSizePolicy valueP = frame_value->sizePolicy();

	if (ratio == 0) {
		keyP.setHorizontalStretch(0);
		label_key->setSizePolicy(keyP);
		valueP.setHorizontalStretch(0);
		frame_value->setSizePolicy(valueP);
	}
	else
	{
		if (ratio >= 1) {
			keyP.setHorizontalStretch(ratio);
			label_key->setSizePolicy(keyP);
			valueP.setHorizontalStretch(1);
			frame_value->setSizePolicy(valueP);
		}
		else{
			keyP.setHorizontalStretch(1);
			label_key->setSizePolicy(keyP);
			valueP.setHorizontalStretch(1/ratio);
			frame_value->setSizePolicy(valueP);
		}
	}
	return true;
}


QWidget* CustomKeyValueWidget::getValueWidget()
{
	return currentValueWidget;
}

void CustomKeyValueWidget::setHighLight(bool isHightLighted)
{
	if (isHightLighted) {
		frame_main->setFrameShape(QFrame::Box);
		frame_main->setFrameShadow(QFrame::Plain);
		QString styleSheet = "QFrame#frame_main{ border: 2px dashed rgb(%RED%,%GREEN%,%BLUE%); border - radius: 1px; }";

		QString redString = QString::number(highLightColor.red());
		QString greenString = QString::number(highLightColor.green());
		QString blueString = QString::number(highLightColor.blue()); 
		//QString alphaString = QString::number(highLightColor.alpha());
		styleSheet = styleSheet.replace("%RED%", redString);
		styleSheet = styleSheet.replace("%GREEN%", greenString);
		styleSheet = styleSheet.replace("%BLUE%", blueString);
		//styleSheet = styleSheet.replace("%ALPHA%", alphaString);

		frame_main->setStyleSheet(styleSheet);
		//QString styleSheet = "QFrame{border - radius:10px; border:10px solid rgb(255,0, 0);}";
		////QString styleSheet = "QFrame{border - radius:10px; border:2px solid rgb(%RED%,%GREEN%,%BLUE%);}";
		////QString styleSheet = "QFrame{border - radius:10px; border:2px rgb(%RED%,%GREEN%,%BLUE%,%ALPHA%)}";
		
		////QString alphaString = QString::number(highLightColor.alpha());

		////styleSheet = styleSheet.replace("%ALPHA%",alphaString);
		//ui.frame_main->setStyleSheet(styleSheet);
	}
	else
	{
		frame_main->setFrameShape(QFrame::Shape::NoFrame);
		frame_main->setFrameShadow(QFrame::Shadow::Plain);
		frame_main->setStyleSheet("");
	}
	highLighted = isHightLighted;
}

void CustomKeyValueWidget::setHighLightColor(QColor color)
{
	highLightColor = color;
	setHighLight(highLighted);
}
