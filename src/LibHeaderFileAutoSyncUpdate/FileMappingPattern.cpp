#include "FileMappingPattern.h"
FileMappingPattern::FileMappingPattern() {

}

FileMappingPattern::FileMappingPattern(QString patternName)
	:m_patternName(patternName)
{
}
FileMappingPattern::FileMappingPattern(const FileMappingPattern&o){
	m_patternName = o.m_patternName;
	list_task = o.list_task;
}
FileMappingPattern::FileMappingPattern(FileMappingPattern&&o)noexcept
{
	m_patternName =o.m_patternName;
	list_task.swap(o.list_task);
}
FileMappingPattern& FileMappingPattern::operator=(const FileMappingPattern&o)
{
	m_patternName = o.m_patternName;
	list_task = o.list_task;
	return *this;
}

QList<FileMappingTask>  FileMappingPattern::getTaskList()
{
	return list_task;
}

QString FileMappingPattern::name()
{
	return m_patternName;
}

void FileMappingPattern::setName(QString name)
{
	m_patternName = name;
}

void FileMappingPattern::updateTaskList(QList<FileMappingTask>& list)
{
	list_task = list;
}


