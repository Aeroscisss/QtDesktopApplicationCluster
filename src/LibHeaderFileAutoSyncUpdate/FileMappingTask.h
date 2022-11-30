#pragma once
#include <QString>
class FileMappingTask
{
public:
	FileMappingTask();
	~FileMappingTask()=default;
	FileMappingTask(const FileMappingTask&);
	FileMappingTask(FileMappingTask&&)noexcept;
	FileMappingTask& operator=(const FileMappingTask&);
	QString name();
	void setName(QString name);
	void setScript(QString script);
private:
	QString taskName;
	QString rawTaskScript;
};

