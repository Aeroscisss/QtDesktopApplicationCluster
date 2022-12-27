#pragma once
#include "ImprovedThreadControl.h"
#include <QObject>
#include <QThread>
#include <QDebug>
#include <memory>
#include <thread>  

ImprovedThreadBase::ThreadState ImprovedThreadControl::threadState(ImprovedThreadBase& thread)
{
	ImprovedThreadBase::ThreadState s = ImprovedThreadBase::ThreadState::Thread_Stoped;
	if (!thread.thread.isRunning()) {
		s = ImprovedThreadBase::ThreadState::Thread_Stoped;
	}
	else {
		if (thread.pauseFlag)
			s = ImprovedThreadBase::ThreadState::Thread_Paused;
		else
			s = ImprovedThreadBase::ThreadState::Thread_Running;
	}
	return s;
}

void ImprovedThreadControl::startThread(ImprovedThreadBase& base)
{
	if (!base.thread.isRunning()) {
		base.thread.start();
		base.stopFlag = false;
		QString msg;
		msg += "[ImprovedThreadControl]:"
			+QString("started thread Name[")
			+ base.name()+"], Thread ID = ["
			+ QString::fromStdString(std::to_string(reinterpret_cast<uintptr_t>(base.thread.currentThreadId()))) 
			+QString("]") ;
		qDebug() << msg;
	}
}

void ImprovedThreadControl::pauseThread(ImprovedThreadBase& base)
{
	if (base.thread.isRunning()) {
		base.pauseFlag = true;
		QString msg;
		msg += "[ImprovedThreadControl]:"
			+ QString("pause thread Name[")
			+ base.name() + "], Thread ID = ["
			+ QString::fromStdString(std::to_string(reinterpret_cast<uintptr_t>(base.thread.currentThreadId())) + "]");
			qDebug() << msg;
	}
}

void ImprovedThreadControl::resumeThread(ImprovedThreadBase& base)
{
	if (base.thread.isRunning()) {
		base.pauseFlag = false;
		base.cond_pauseThread.wakeAll();
		QString msg;
		msg += "[ImprovedThreadControl]:"
			+ QString("resume thread Name[")
			+ base.name() + "], Thread ID = ["
			+ QString::fromStdString(std::to_string(reinterpret_cast<uintptr_t>(base.thread.currentThreadId())) + "]");
			qDebug() << msg;
	}
}

void ImprovedThreadControl::stopThread(ImprovedThreadBase& base)
{
	if (base.thread.isRunning()) {
		base.thread.requestInterruption();
		base.stopFlag = true;
		base.cond_pauseThread.wakeAll();
		base.thread.quit();
		base.thread.wait();
		QString msg;
		msg += "[ImprovedThreadControl]:"
			+ QString("stop thread Name[")
			+ base.name() + "], Thread ID = ["
			+ QString::fromStdString(std::to_string(reinterpret_cast<uintptr_t>(base.thread.currentThreadId())) + "]");
			qDebug() << msg;
	}
}
