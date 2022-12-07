#pragma once
#include <QString>
#include <QList>
#include <QMap>
#include <mutex>
#include <QJsonObject>
#include "FileMappingTask.h"
class FileMappingPattern
{
public:
	FileMappingPattern();
	FileMappingPattern(QJsonObject obj);
	FileMappingPattern(QString patternName);
	~FileMappingPattern()=default;
	FileMappingPattern(const FileMappingPattern&);
	FileMappingPattern(FileMappingPattern&&)noexcept;
	FileMappingPattern& operator=(const FileMappingPattern&);
	QList<FileMappingTask> getTaskList();
	QString name();
	void setName(QString name);
	void updateTaskList(QList<FileMappingTask>&list_task);
	QJsonObject toJsonObj();
private:
	QString m_patternName;
	std::mutex mutex_tasks;
	QList<FileMappingTask>list_task;
};

