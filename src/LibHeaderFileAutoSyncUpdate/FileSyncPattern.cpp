#include "FileSyncPattern.h"
#include <QJsonArray>
FileSyncPattern::FileSyncPattern() {

}

FileSyncPattern::FileSyncPattern(QJsonObject obj)
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
			list_task.append(FileSyncTask(iter->toObject()));
		}
	}
}

FileSyncPattern::FileSyncPattern(QString patternName)
	:m_patternName(patternName)
{
}
FileSyncPattern::FileSyncPattern(const FileSyncPattern&o){
	m_patternName = o.m_patternName;
	list_task = o.list_task;
}
FileSyncPattern::FileSyncPattern(FileSyncPattern&&o)noexcept
{
	m_patternName =o.m_patternName;
	list_task.swap(o.list_task);
}
FileSyncPattern& FileSyncPattern::operator=(const FileSyncPattern&o)
{
	m_patternName = o.m_patternName;
	list_task = o.list_task;
	return *this;
}

QList<FileSyncTask>  FileSyncPattern::getTaskList()
{
	return list_task;
}

QString FileSyncPattern::name()
{
	return m_patternName;
}

void FileSyncPattern::setName(QString name)
{
	m_patternName = name;
}

void FileSyncPattern::updateTaskList(QList<FileSyncTask>& list)
{
	std::lock_guard<std::mutex>locker(mutex_tasks);
	list_task = list;
}

void FileSyncPattern::addTask(FileSyncTask task)
{
	std::lock_guard<std::mutex>locker(mutex_tasks);
	list_task.append(task);
}

void FileSyncPattern::removeTask(int index)
{
	std::lock_guard<std::mutex>locker(mutex_tasks);
	if (index>0 && index<list_task.size()) {
		list_task.removeAt(index);
	}
}

QJsonObject FileSyncPattern::toJsonObj()
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

QString FileSyncPattern::toConsoleString()
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

QString FileSyncPattern::toString()
{
	QString out;
	out += "PatternName[" +m_patternName + "]:";
	std::lock_guard<std::mutex>locker(mutex_tasks);
	for (auto iter = list_task.begin(); iter != list_task.end(); iter++) {
		out + iter->toString() + "\n";
	}
	return out;
}


