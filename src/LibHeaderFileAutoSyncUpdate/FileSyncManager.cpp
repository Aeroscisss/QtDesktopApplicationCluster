#include "FileSyncManager.h"
#include "FileSyncPattern.h"
#include "GlobalMessageRepost.h"
#include <QJsonDocument>
#include <QJsonArray>
#include "CustomQToolKit/QJsonIO.h"
#include "GlobalSettings.h"
#include <QDir>
#include <QCoreApplication>
#include "CustomQToolKit/ImprovedThreadControl.h"
FileSyncManager& FileSyncManager::Instance() {
	static std::unique_ptr<FileSyncManager>instance_ptr =
		std::unique_ptr<FileSyncManager>(new FileSyncManager);
	return *instance_ptr;
}
FileSyncManager::FileSyncManager() {
	this->setObjectName("FileSyncManager");
	this->moveToThread(&m_thread.thread);
	ImprovedThreadControl::startThread(m_thread);
}
FileSyncManager::~FileSyncManager() {

}
QString FileSyncManager::newPatternNameSuggestion()
{
	std::lock_guard<std::mutex>locker(mutex_patterns);//多线程锁
	QString nameBase = "New Pattern ";
	QString name;
	bool breakFlag = false;
	for (int i = 1; !breakFlag; i++) {
		name = nameBase + QString::number(i);
		if (!map_fileSyncPattern.contains(name)) {
			breakFlag = true;
		}
	}
	return name;
}
bool FileSyncManager::getPattern(QString patternName, FileSyncPattern&pattern)
{
	std::lock_guard<std::mutex>locker(mutex_patterns);//多线程锁
	auto iter = map_fileSyncPattern.find(patternName);
	if(iter==map_fileSyncPattern.end())
		return false;
	pattern = iter.value();
	return true;
}
bool FileSyncManager::updatePattern(QString patternName, FileSyncPattern& pattern)
{
	try {
		std::lock_guard<std::mutex>locker(mutex_patterns);//多线程锁
		auto iter = map_fileSyncPattern.find(patternName);
		if (iter == map_fileSyncPattern.end()) {
			throw std::exception("No Such Pattern");
		}
		map_fileSyncPattern[iter.key()] = pattern;
		GlobalMessageRepost::Instance().sendNewMsg("Update Pattern[" + patternName + "]. ", 1);
		return true;
	}
	catch (std::exception e) {
		QString msg = "Fail Update Pattern[" + patternName + "]. ";
		GlobalMessageRepost::Instance().sendNewMsg(msg + e.what());
		return false;
	}
}
QStringList FileSyncManager::getPatternNames()
{
	std::lock_guard<std::mutex>locker(mutex_patterns);//多线程锁
	QStringList list;
	for(auto iter=map_fileSyncPattern.begin();iter!=map_fileSyncPattern.end();iter++){
		list.append(iter.key());
	}
	return list;
}
QString FileSyncManager::getCurrentRuleFilePath()
{
	return currentRuleFilePath;
}


//————————————private——————
bool FileSyncManager::openRuleFile(QString filePath)
{
	GlobalMessageRepost::Instance().sendNewMsg("Try Open Rule File "+filePath, 1);
	try {
		std::lock_guard<std::mutex>locker(mutex_patterns);//多线程锁
		QJsonIO jsonIo;
		QJsonDocument doc;
		if (!jsonIo.readJsonFile(filePath, doc)) {
			throw std::exception(jsonIo.errorMsg.toStdString().c_str());
		}
		QJsonObject obj = doc.object();
		QJsonArray patternList;
		if (obj.contains("patternList"))
		{
			patternList = obj["patternList"].toArray();
			QMap<QString, FileSyncPattern>patternMap;
			for (auto iter = patternList.begin(); iter != patternList.end(); iter++) {
				QJsonObject patternObj = iter->toObject();
				FileSyncPattern pattern(patternObj);
				patternMap.insert(pattern.name(), pattern);
			}
			map_fileSyncPattern = patternMap;
		}
		currentRuleFilePath = filePath;
		GlobalSettings::Instance().absoluteLatestRuleFilePath = QDir(filePath).absolutePath();
		QString currDir = QDir::currentPath();
		QDir dir(currDir);
		GlobalSettings::Instance().reletiveLatestRuleFilePath = dir.relativeFilePath(GlobalSettings::Instance().absoluteLatestRuleFilePath);
		emit sig_fileSyncManager_ruleFileOpened();
	}
	catch (std::exception e) {
		QString msg = "Fail Open File. ";
		GlobalMessageRepost::Instance().sendNewMsg(msg + e.what());
		return false;
	}

	return true;
}
bool FileSyncManager::saveRuleFile(QString filePath)
{
	GlobalMessageRepost::Instance().sendNewMsg("saveRuleFile()", 1);
	try {
		std::lock_guard<std::mutex>locker(mutex_patterns);//多线程锁
		QJsonIO jsonIo;
		QJsonDocument doc;
		QJsonObject obj;
		QJsonArray patternList;
		for (auto iter = map_fileSyncPattern.begin(); iter != map_fileSyncPattern.end(); iter++) {
			patternList.append(iter->toJsonObj());
		}
		obj.insert("patternList", patternList);
		doc.setObject(obj);
		if (!jsonIo.writeJsonFile(filePath, doc)) {
			throw std::exception(QString("Unable to write file\n×[" + filePath + "]").toStdString().c_str());
		}
		QString msg = "File Saved Success.";
		if (currentRuleFilePath != filePath) {
			msg += "\nFile Path [" + filePath + "]";
		}
		GlobalMessageRepost::Instance().sendNewMsg(msg);
		currentRuleFilePath = filePath;
		emit sig_fileSyncManager_ruleFileOpened();
		return true;
	}
	catch (std::exception e) {
		QString msg = "Fail Save File. ";
		GlobalMessageRepost::Instance().sendNewMsg(msg + e.what());
		return false;
	}
}
void FileSyncManager::closeCurrRuleFile()
{
	std::lock_guard<std::mutex>locker(mutex_patterns);//多线程锁
	map_fileSyncPattern.clear();
	currentRuleFilePath.clear();
	emit sig_fileSyncManager_ruleFileClosed();
}

bool FileSyncManager::deletePattern(QString patternName)
{
	try {
		std::lock_guard<std::mutex>locker(mutex_patterns);//多线程锁
		if (patternName.isEmpty()) {
			throw std::exception("Pattern Name is Empty");
		}
		if (!map_fileSyncPattern.contains(patternName)) {
			throw std::exception("Pattern already Exist");
		}
		else {
			map_fileSyncPattern.remove(patternName);
		}
		QString msg = "Delete Pattern [" + patternName + "].";
		GlobalMessageRepost::Instance().sendNewMsg(msg, GlobalMessageRepost::MsgDst::MainWindowUserMsgBrowser);
		emit sig_fileSyncManager_patternUpdated();
		return true;
	}
	catch (std::exception e) {
		QString msg = "Fail Delete Pattern [" + patternName + "]." + e.what();
		GlobalMessageRepost::Instance().sendNewMsg(msg, GlobalMessageRepost::MsgDst::MainWindowUserMsgBrowser);
		return false;
	}
}

void FileSyncManager::printRuleFileToConsole()
{
	std::lock_guard<std::mutex>locker(mutex_patterns);//多线程锁
	QString stringOut;
	for (auto iter = map_fileSyncPattern.begin(); iter != map_fileSyncPattern.end(); iter++) {
		stringOut+=iter->toConsoleString();
	}
	printf_s("\n");
	printf_s(stringOut.toStdString().c_str());
}
bool FileSyncManager::createNewPattern(QString patternName)
{
	try {
		if (patternName.isEmpty()) {
			throw std::exception("Pattern Name is Empty");
		}
		{
			std::lock_guard<std::mutex>locker(mutex_patterns);//多线程锁
			auto iter = map_fileSyncPattern.find(patternName);
			if (iter != map_fileSyncPattern.end()) {
				throw std::exception("Pattern already Exist");
			}
			else {
				map_fileSyncPattern[patternName] = FileSyncPattern(patternName);
			}
		}
		QString msg = "Create New Pattern [" + patternName + "].";
		GlobalMessageRepost::Instance().sendNewMsg(msg, GlobalMessageRepost::MsgDst::MainWindowUserMsgBrowser);
		emit sig_fileSyncManager_patternUpdated();
		GlobalMessageRepost::Instance().sendNewMsg("Mutex Bug test", 1);
		return true;
	}
	catch (std::exception e) {
		QString msg = "Fail Create New Pattern [" + patternName + "]."+e.what();
		GlobalMessageRepost::Instance().sendNewMsg(msg, GlobalMessageRepost::MsgDst::MainWindowUserMsgBrowser);
		return false;
	}
}

void FileSyncManager::rec_openRuleFile(QString path) {
	openRuleFile(path);
}
void FileSyncManager::rec_saveRuleFile(QString path)
{
	saveRuleFile(path);
}

void FileSyncManager::rec_closeCurrRuleFile()
{
	closeCurrRuleFile();
}

void FileSyncManager::rec_createNewPattern(QString patternName) {
	createNewPattern(patternName);
}
void FileSyncManager::rec_deletePattern(QString name)
{
	deletePattern(name);
}
void FileSyncManager::rec_printPatternsToConsole()
{
	printRuleFileToConsole();
}