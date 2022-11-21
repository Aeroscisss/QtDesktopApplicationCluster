#pragma once
/*
	 ___________________
	/					\
	| shared   widget	|
	\__________________/
*/
#ifndef IMPROVED_THREAD_CONTROL_H
#define IMPROVED_THREAD_CONTROL_H

#include "customQToolKit/ImprovedThreadBase.h"
class ImprovedThreadControl
{
public:

public:
	static ImprovedThreadBase::ThreadState threadState(ImprovedThreadBase& thread);
	static void startThread(ImprovedThreadBase& base);
	static void pauseThread(ImprovedThreadBase& base);
	static void resumeThread(ImprovedThreadBase& base);
	static void stopThread(ImprovedThreadBase& base);
private:
};
#endif // !IMPROVED_THREAD_CONTROL_H
