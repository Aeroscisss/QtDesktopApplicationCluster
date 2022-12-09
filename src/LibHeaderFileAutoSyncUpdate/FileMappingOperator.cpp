#include "FileMappingOperator.h"
#include <memory>
FileMappingOperator& FileMappingOperator::Instance()
{
	static std::unique_ptr<FileMappingOperator>instance_ptr=std::unique_ptr<FileMappingOperator>(new FileMappingOperator);
	return *instance_ptr;
}

FileMappingOperator::FileMappingOperator()
{
	threadIsInterrupted = false;
	m_thread = std::make_unique< std::thread>();
}

FileMappingOperator::~FileMappingOperator()
{
	m_thread->join();
}

void FileMappingOperator::threadLoopRun()
{
	while (!FileMappingOperator::Instance().threadIsInterrupted)
	{

	}
}

void FileMappingOperator::addRule(FileMappingRule)
{
	std::lock_guard<std::mutex>locker(mutex_rules);
	queue_rules
}
