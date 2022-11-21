#include "pch.h"
#include "CustomMsgBrowserWidget.h"
#include <QTime>
CustomMsgBrowserWidget::CustomMsgBrowserWidget(QWidget *parent)
	: CustomAbstractQWidget(parent)
{
	ui.setupUi(this);
	defaultFontPointSize= 15;
	setMsgTimePrifixAddition(true);
	ui.textBrowser->setFontPointSize(defaultFontPointSize);
}

CustomMsgBrowserWidget::~CustomMsgBrowserWidget()
{}

void CustomMsgBrowserWidget::setMsgTimePrifixAddition(bool addFix)
{
	addTimePrefix = addFix;
}

void CustomMsgBrowserWidget::setCustomPrifixAddition(bool addCustomFix, QString text)
{
	addCustomPrefix = addCustomFix;
	customPrifix = text;
}

void CustomMsgBrowserWidget::appendMsg(QString msg)
{
	QString preFix;
	if (addTimePrefix)
		preFix+= QTime::currentTime().toString("[hh:mm:ss]");
	if (addCustomPrefix)
		preFix += customPrifix;
	if (!preFix.isEmpty())
		preFix += " ";
	ui.textBrowser->append(preFix+msg);
}
void CustomMsgBrowserWidget::setLayoutDirection(bool isBottomUp)
{
	if (isBottomUp) {
	}
}
void CustomMsgBrowserWidget::rec_appendMsg(QString msg) {
	appendMsg(msg);
}

void CustomMsgBrowserWidget::on_btn_addFontSize_clicked()
{
	int size = ui.textBrowser->fontPointSize();
	size++;
	appendMsg(tr("font point size : ") + QString::number(size));
	ui.textBrowser->setFontPointSize(size);
}

void CustomMsgBrowserWidget::on_btn_subFontSize_clicked()
{
	int size = ui.textBrowser->fontPointSize();
	if (size > 1) {
		size--;
		appendMsg(tr("font point size : ") + QString::number(size));
		ui.textBrowser->setFontPointSize(size);
	}
	else {
		appendMsg(tr("reached smallest font point size"));
	}
	
}
void CustomMsgBrowserWidget::on_btn_resetFontSize_clicked()
{
	ui.textBrowser->setFontPointSize(defaultFontPointSize);
	appendMsg(tr("font point size set to default(") + QString::number(defaultFontPointSize)+")");
}
void CustomMsgBrowserWidget::on_btn_clearTextBrowser_clicked()
{
	ui.textBrowser->clear();
}