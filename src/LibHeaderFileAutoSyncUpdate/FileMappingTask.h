#pragma once
class FileMappingTask
{
public:
	FileMappingTask();
	~FileMappingTask()=default;
	FileMappingTask(const FileMappingTask&);
	FileMappingTask(FileMappingTask&&) = delete;
	FileMappingTask& operator=(const FileMappingTask&);
private:

};

