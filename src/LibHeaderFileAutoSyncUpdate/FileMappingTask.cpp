#include "FileMappingTask.h"

FileMappingTask::FileMappingTask()
{
}
FileMappingTask::FileMappingTask(const FileMappingTask&o)
{
	taskName = o.taskName;
	rawTaskScript = o.rawTaskScript;
}
FileMappingTask::FileMappingTask(FileMappingTask&& o)noexcept
{
	taskName = o.taskName;
	rawTaskScript = o.rawTaskScript;
}
FileMappingTask& FileMappingTask::operator=(const FileMappingTask&o)
{
	taskName = o.taskName;
	rawTaskScript = o.rawTaskScript;
	return *this;
}

QString FileMappingTask::name()
{
	return taskName;
}

void FileMappingTask::setName(QString name)
{
	taskName = name;
}

void FileMappingTask::setScript(QString script)
{
	rawTaskScript = script;
}
