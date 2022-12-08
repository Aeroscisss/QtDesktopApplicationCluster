#include "FileMappingPattern.h"
#include <QJsonArray>
FileMappingPattern::FileMappingPattern() {

}

FileMappingPattern::FileMappingPattern(QJsonObject obj)
{
	if (obj.contains("patternName")) {
		QJsonValue name = obj["patternName"];
		m_patternName = name.toString();
	}
	if (obj.contains("taskList")) {
		QJsonValue taskList_v = obj["taskList"];
		QJsonArray taskList = taskList_v.toArray();
		std::lock_guard<std::mutex>locker(mutex_tasks);
		for(auto iter=taskList.begin();iter!=taskList.end();iter++){
			list_task.append(FileMappingTask(iter->toObject()));
		}
	}
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
	std::lock_guard<std::mutex>locker(mutex_tasks);
	list_task = list;
}

QJsonObject FileMappingPattern::toJsonObj()
{
	QJsonObject obj;
	QJsonArray taskList;
	std::lock_guard<std::mutex>locker(mutex_tasks);
	for (auto iter = list_task.begin(); iter != list_task.end(); iter++) {
		taskList.append(iter->toJsonObj());
	}
	obj.insert("patternName",m_patternName);
	obj.insert("taskList", taskList);
	return obj;
}

QString FileMappingPattern::toConsoleString()
{
	QString out;
	out += "PatternName[" + m_patternName + "]:\n";
	std::lock_guard<std::mutex>locker(mutex_tasks);
	for (auto iter = list_task.begin(); iter != list_task.end(); iter++) {
		QString taskString=iter->toConsoleString();
		out += taskString;
	}
	return out;
}

QString FileMappingPattern::toString()
{
	QString out;
	out += "PatternName[" +m_patternName + "]:";
	std::lock_guard<std::mutex>locker(mutex_tasks);
	for (auto iter = list_task.begin(); iter != list_task.end(); iter++) {
		out + iter->toString() + "\n";
	}
	return out;
}


