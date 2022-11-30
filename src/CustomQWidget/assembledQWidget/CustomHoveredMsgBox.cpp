#include "CustomHoveredMsgBox.h"
#include <QGraphicsOpacityEffect>
#include <QEventLoop>
#include <QTimer>
#include <QCursor>
#include <QPoint>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QStyleOption>
#include <QPainter>
#include <iostream>

CustomHoveredMsgBox::CustomHoveredMsgBox(QString text, QWidget* in_targetWidget, QWidget* parent)
	: QLabel(parent),targetWidget(in_targetWidget)
{
	connect(this, SIGNAL(sig_showFuncIsCalled()), this, SLOT(rec_afterWidgetIsCalledToShow()));
	this->setAttribute(Qt::WA_TransparentForMouseEvents); //鼠标穿透,必须先设置
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);//为了重写paintEvent必须设置背景透明
	this->setAttribute(Qt::WA_DeleteOnClose);//关闭时自动销毁
	this->setAttribute(Qt::WA_ShowWithoutActivating, true);//不获得焦点
	this->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
	
	/*QGridLayout* layout = new QGridLayout(this);
	layout->addWidget(&msgLabel);*/
	QPalette currrPalette = qApp->palette();
	QPalette palette;
	baseColor = currrPalette.color(QPalette::ColorGroup::Normal, QPalette::ColorRole::Shadow);
	textColor = currrPalette.color(QPalette::ColorGroup::Normal, QPalette::ColorRole::ToolTipText);
	//this->hide();//先隐藏，必须手动launch
	setText(text);
	setElapseArg();
	//setFixedSize(1, 1);
}

CustomHoveredMsgBox::~CustomHoveredMsgBox()
{
}

void CustomHoveredMsgBox::setElapseArg(bool in_elapse, int startValue , int endValue, int milisecond, QEasingCurve curve )
{
	elapse = in_elapse;
	if (elapse) {
		/*QGraphicsOpacityEffect* widgetOpacity = new QGraphicsOpacityEffect(this);
		widgetOpacity->setOpacity(1);
		this->setGraphicsEffect(widgetOpacity);*/
		//animation = new QPropertyAnimation(widgetOpacity, "opacity");
		//animation = new QPropertyAnimation(this, "windowOpacity");
		animation.setTargetObject(this);
		animation.setPropertyName("windowOpacity");
		if (startValue < 0)
			startValue = 0;
		if (startValue > 100)
			startValue = 100;
		if (endValue < 0)
			endValue = 0;
		if (endValue > 100)
			endValue = 100;
		float sV = (float)startValue / 100.f;
		float eV = (float)endValue / 100.f;
		animation.setStartValue(sV);
		animation.setEndValue(eV);
		animation.setDuration(milisecond);
		animation.setEasingCurve(curve);
		connect(&animation, SIGNAL(finished()), this, SLOT(rec_afterAnimationFinished()));//效果显示完后关闭
	}

}

void CustomHoveredMsgBox::setCursorFollowed(bool followed){
	followCurser = followed;
}

bool CustomHoveredMsgBox::setGeometry(int x, int y)
{
	if (x < 0 || y < 0) {
		return false;
	}
	absGeoX = x;
	absGeoY = y;
	//move(x, y);
	return true;
}


bool CustomHoveredMsgBox::setRelativePosition(float wRatio, float hRatio)
{
	if (wRatio < 0 || wRatio>1|| hRatio < 0 ||hRatio>1)
		return false;
	ratioX = wRatio;
	ratioY = hRatio;
	return true;
}

void CustomHoveredMsgBox::setMaxOpacityPercent(int in_opacityPercent)
{
	if (in_opacityPercent > 100)
		in_opacityPercent = 100;
	if (in_opacityPercent < 0)
		return;
	float floatPercent = (float)in_opacityPercent / (float)100;
	QPalette currrPalette = qApp->palette();
	QPalette palette;
	QColor color = baseColor;
	color.setAlpha(floatPercent * 255);
	palette.setColor(QPalette::Background, color);
	this->setPalette(palette);
	opacityPercent = in_opacityPercent;
}

void CustomHoveredMsgBox::setTextColor(QColor text)
{
	textColor = text;
}

void CustomHoveredMsgBox::setBaseColor(QColor base)
{
	baseColor = base;
}

void CustomHoveredMsgBox::closeEvent(QCloseEvent* e)
{
}

void CustomHoveredMsgBox::showEvent(QShowEvent* e)
{
	emit sig_showFuncIsCalled();
}

void CustomHoveredMsgBox::paintEvent(QPaintEvent* event)
{
	QPalette currrPalette = qApp->palette();
	QPalette palette;
	QColor color = currrPalette.color(QPalette::ColorGroup::Normal, QPalette::ColorRole::ToolTipBase);
	color.setAlpha((float)((float)opacityPercent / 100.f) * 255);
	//palette.setColor(QPalette::Background, QColor(color));

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(QBrush(baseColor));
	painter.setPen(Qt::transparent);
	QRect rect = this->rect();
	rect.setWidth(rect.width() - 1);
	rect.setHeight(rect.height() - 1);
	// rect: 绘制区域  ：圆角弧度3
	painter.drawRoundedRect(rect, 5, 5);

	QWidget::paintEvent(event);
	
	QRect r;
	QPainter p(this);
	p.setPen(textColor);
	p.drawText(7,6, rect.width()+1, rect.height()+1, 0, text(), &r);
}

void CustomHoveredMsgBox::refreshGeometry()
{
	QRect nowRect = geometry();
	nowRect.setWidth(nowRect.width() + 18);
	resize(nowRect.width(), nowRect.height());

	if (followCurser) {
		//tested
		QPoint currCursorGeo = QCursor().pos();
		QRect nowRect = geometry();
		int x = currCursorGeo.x() - (0.5 * nowRect.width());
		int y = currCursorGeo.y() - (0.5 * nowRect.height());
		move(x,y);
		//tested
	}
	else {
		if (targetWidget != nullptr) {
			//testing
			QRect tarGeo = targetWidget->geometry();
			QRect thisGeo = geometry();
			int baseX = tarGeo.x() ;
			int baseY = tarGeo.y() ;
			int offsetX = ratioX * tarGeo.width();
			int offsetY = ratioY * tarGeo.height();
			int midX = 0.5f * (float)thisGeo.width();
			int midY = 0.5f * (float)thisGeo.height();
			int x = baseX+ offsetX - midX;
			int y = baseY+ offsetY - midY ;
			move(x, y);
		}
		else {
			if (absGeoX != -99 && absGeoY != -99) {
				move(absGeoX, absGeoY);
			}
			else {
				QRect deskRect = qApp->desktop()->availableGeometry();
				int x = deskRect.width() * ratioX;
				int y = deskRect.height() * ratioY;
				move(x, y);
			}
		}
	}
}

void CustomHoveredMsgBox::rec_afterWidgetIsCalledToShow()
{
	setMinimumHeight(30);
	refreshGeometry();
	if (elapse) {
		animation.start();
	}
	//qDebug() << geometry();
}


void CustomHoveredMsgBox::rec_afterAnimationFinished() {
	/*QEventLoop eventloop;
	QTimer::singleShot(3000, &eventloop, SLOT(quit()));
	eventloop.exec();*/
	close();
}
