#pragma once
#include <QString>
#include <QList>
#include <QQueue>
#include "FileMappingRule.h"
#include <Thread>
#include <memory>
#include <mutex>
class FileMappingOperator
{
public:
	static FileMappingOperator& Instance();
	~FileMappingOperator();
	static void threadLoopRun();
	bool threadIsInterrupted;
	void addRule(FileMappingRule);
private:
	FileMappingOperator();
	std::mutex mutex_rules;
	QQueue<FileMappingRule>queue_rules;
	std::unique_ptr<std::thread>m_thread = nullptr;
};

