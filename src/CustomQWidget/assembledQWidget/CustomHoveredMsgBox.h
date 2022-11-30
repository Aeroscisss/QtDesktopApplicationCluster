#pragma once

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QApplication>
#include <QShowEvent>
#include <QPropertyAnimation>
#include <QGridLayout>
#include <QColor>
class CustomHoveredMsgBox : public QLabel
{
	Q_OBJECT

public:
	CustomHoveredMsgBox(QString text, QWidget* targetWidget = nullptr,QWidget* parent = nullptr);
	~CustomHoveredMsgBox();
	void setElapseArg(bool elapse=true,int startValue=100,int endValue=0,int milisecond=3000, QEasingCurve curve=QEasingCurve::InQuint);
	void setCursorFollowed(bool followed);
	bool setRelativePosition(float wRatio, float hRatio);
	bool setGeometry(int x,int y);
	void setMaxOpacityPercent(int opacityPercent);
	void setTextColor(QColor text);
	void setBaseColor(QColor base);
	QPropertyAnimation animation;
protected:
	void closeEvent(QCloseEvent*e)override;
	void showEvent(QShowEvent* e)override;
	void paintEvent(QPaintEvent* e)override;
signals:
	void sig_showFuncIsCalled();
private:
	void refreshGeometry();
private:
	QWidget* targetWidget = nullptr;
	bool elapse = true;
	int opacityPercent = 85;//默认85%不透明度
	QTimer elapseTimer;

	//int maxColorAlpha=50;

	bool followCurser=false;
	float ratioX=0.5;
	float ratioY=0.2;
	int absGeoX=-99;
	int absGeoY=-99;

	QColor baseColor;
	QColor textColor;
private slots:
	void rec_afterAnimationFinished();
	void rec_afterWidgetIsCalledToShow();
};
