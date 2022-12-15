#pragma once
#include <QString>
#include <QList>
#include <queue>
#include "FileSyncRule.h"
#include <Thread>
#include <memory>
#include <mutex>
#include <atomic>
#include "CustomCppToolKit/MultiThreadQueue.h"
class FileSyncOperator
{
public:
	static FileSyncOperator& Instance();
	~FileSyncOperator();
	std::unique_ptr<MultiThreadQueue<FileSyncRule> >ruleQueue;
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
	void applyRule(FileSyncRule rule);
	void convertPathValueIntoFileInfo(QStringList files,QMap<QString, CustomFileInfo>&fileInfoMap);
	void findAllFilesUnderFolder(QString folderPath, QMap<QString, CustomFileInfo>& fileInfoMap);
	QString smallFileMd5(QString path);
	QString bigFileMd5(QString path);
	void regexCheck(QMap<QString, CustomFileInfo>& map,QStringList regexList);
	bool copyFile(QString srcPath, QString dstPath, bool coverFileIfExist);
	FileSyncOperator();
	std::thread m_thread ;
};

