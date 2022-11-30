#pragma once
#include "ImprovedThreadControl.h"
#include <QObject>
#include <QThread>
#include <memory>
#include <thread>  
#include "PiCore/PiStandaloneReport.h"

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
		Pi::PiStandaloneReport::traceReport("ImprovedThreadControl", std::string("started thread Name[") + base.name().toStdString() + "], Thread ID = [" + std::to_string(reinterpret_cast<uintptr_t>(base.thread.currentThreadId())) + "]");
	}
}

void ImprovedThreadControl::pauseThread(ImprovedThreadBase& base)
{
	if (base.thread.isRunning()) {
		base.pauseFlag = true;
		Pi::PiStandaloneReport::traceReport("ImprovedThreadControl", std::string("paused thread Name[") + base.name().toStdString() + "], Thread ID = [" + std::to_string(reinterpret_cast<uintptr_t>(base.thread.currentThreadId())) + "]");
	}
}

void ImprovedThreadControl::resumeThread(ImprovedThreadBase& base)
{
	if (base.thread.isRunning()) {
		base.pauseFlag = false;
		base.cond_pauseThread.wakeAll();
		Pi::PiStandaloneReport::traceReport("ImprovedThreadControl", std::string("resume thread Name[") + base.name().toStdString() + "], Thread ID = [" + std::to_string(reinterpret_cast<uintptr_t>(base.thread.currentThreadId())) + "]");
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
		Pi::PiStandaloneReport::traceReport("ImprovedThreadControl", std::string("stoped thread Name[") + base.name().toStdString() + "], Thread ID = [" + std::to_string(reinterpret_cast<uintptr_t>(base.thread.currentThreadId())) + "]");
	}
}
