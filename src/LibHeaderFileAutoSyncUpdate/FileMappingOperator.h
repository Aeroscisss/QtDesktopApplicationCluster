#pragma once
#include <QString>
#include <QList>
#include <queue>
#include "FileMappingRule.h"
#include <Thread>
#include <memory>
#include <mutex>
#include "CustomCppToolKit/MultiThreadQueue.h"
class FileMappingOperator
{
public:
	static FileMappingOperator& Instance();
	~FileMappingOperator();
	std::unique_ptr<MultiThreadQueue<FileMappingRule> >ruleQueue;
	void threadLoopRun();
	bool threadIsInterrupted=false;
private:
	void applyRule(FileMappingRule rule);
	QString smallFileMd5(QString path);
	QString bigFileMd5(QString path);
	void regexCheck(QStringList& list,QStringList regexList);
	bool copyFile(QString srcPath, QString dstPath, bool coverFileIfExist);
	FileMappingOperator();
	std::unique_ptr<std::thread>m_thread = nullptr;
};

