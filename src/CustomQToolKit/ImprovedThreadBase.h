#pragma once
/*
	 ___________________
	/					\
	| shared   widget	|
	\__________________/
*/
#ifndef IMPROVED_THREAD_BASE_H
#define IMPROVED_THREAD_BASE_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QThread>
#include <QMetaType>
class ImprovedThreadBase :public QObject
{
public:
	enum class LongTimeProcessResult :int {
		LTP_Undefined = -1,
		LTP_Fail = 0,
		LTP_PartlySuccess = 2,
		LTP_Success = 1
	};
	enum class ThreadState
	{
		Thread_Stoped,     ///<停止状态，包括从未启动过和启动后被停止
		Thread_Running,    ///<运行状态
		Thread_Paused      ///<暂停状态
	};
	ImprovedThreadBase() {}
	~ImprovedThreadBase(){}
	void setName(QString name) {threadName = name;}
	QString name() {return threadName;}
public:
	QString threadName;
	QThread thread;
	//线程相关
	QMutex mutex_pauseThread;
	QWaitCondition cond_pauseThread;
	std::atomic_bool pauseFlag = false;
	std::atomic_bool stopFlag = false;
protected:

};
Q_DECLARE_METATYPE(ImprovedThreadBase::LongTimeProcessResult);
#endif // !IMPROVED_THREAD_BASE_H
