#pragma once
#include <QString>
#include <QList>
#include <queue>
#include "FileMappingRule.h"
#include <Thread>
#include <memory>
#include <mutex>
#include <atomic>
#include "CustomCppToolKit/MultiThreadQueue.h"
class FileMappingOperator
{
public:
	static FileMappingOperator& Instance();
	~FileMappingOperator();
	std::unique_ptr<MultiThreadQueue<FileMappingRule> >ruleQueue;
	void threadLoopRun();
	std::atomic<bool> threadIsInterrupted;
private:
	struct CustomFileInfo {
		CustomFileInfo() {}
		CustomFileInfo(QString path)
			:filePath(path) {}
		bool isFileFlag = false;
		QString filePath;
		QString fileCompleteName;
		QString fileBaseName;
		QString md5;
		quint64 fileByteSize = 0;
	};
	static void ruleOpLoop();
	void applyRule(FileMappingRule rule);
	void convertPathValueIntoFileInfo(QStringList files,QMap<QString, CustomFileInfo>&fileInfoMap);
	void findAllFilesUnderFolder(QString folderPath, QMap<QString, CustomFileInfo>& fileInfoMap);
	QString smallFileMd5(QString path);
	QString bigFileMd5(QString path);
	void regexCheck(QMap<QString, CustomFileInfo>& map,QStringList regexList);
	bool copyFile(QString srcPath, QString dstPath, bool coverFileIfExist);
	FileMappingOperator();
	std::thread m_thread ;
};

