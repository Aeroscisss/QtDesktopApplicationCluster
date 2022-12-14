#include "FileMappingManager.h"
#include "FileMappingPattern.h"
#include "GlobalMessageRepost.h"
#include <QJsonDocument>
#include <QJsonArray>
#include "CustomQToolKit/QJsonIO.h"
#include "GlobalSettings.h"
FileMappingManager& FileMappingManager::Instance() {
	static std::unique_ptr<FileMappingManager>instance_ptr =
		std::unique_ptr<FileMappingManager>(new FileMappingManager);
	return *instance_ptr;
}
FileMappingManager::FileMappingManager() {

}
FileMappingManager::~FileMappingManager() {

}
QString FileMappingManager::newPatternNameSuggestion()
{
	QString nameBase = "New Pattern ";
	QString name;
	bool breakFlag = false;
	for (int i = 1; !breakFlag; i++) {
		name = nameBase + QString::number(i);
		if (!map_fileMappingPattern.contains(name)) {
			breakFlag = true;
		}
	}
	return name;
}
bool FileMappingManager::getPattern(QString patternName, FileMappingPattern&pattern)
{
	std::lock_guard<std::mutex>locker(mutex_fileMappingPatern);
	auto iter = map_fileMappingPattern.find(patternName);
	if(iter==map_fileMappingPattern.end())
		return false;
	pattern = iter.value();
	return true;
}
QStringList FileMappingManager::getPatternNames()
{
	std::lock_guard<std::mutex>locker(mutex_fileMappingPatern);
	QStringList list;
	for(auto iter=map_fileMappingPattern.begin();iter!=map_fileMappingPattern.end();iter++){
		list.append(iter.key());
	}
	return list;
}
bool FileMappingManager::updatePattern(QString patternName, FileMappingPattern& pattern)
{
	try {
		std::lock_guard<std::mutex>locker(mutex_fileMappingPatern);
		auto iter = map_fileMappingPattern.find(patternName);
		if (iter == map_fileMappingPattern.end())
			throw std::exception("No Such Pattern");
		map_fileMappingPattern[iter.key()] = pattern;
		GlobalMessageRepost::Instance().sendNewMsg("Update Pattern[" + patternName + "]. ", 1);
		return true;
	}
	catch (std::exception e) {
		QString msg = "Fail Update Pattern[" + patternName + "]. ";
		GlobalMessageRepost::Instance().sendNewMsg(msg + e.what());
		return false;
	}
}
bool FileMappingManager::openRuleFile(QString filePath)
{
	try {
		QJsonDocument doc;
		if (!QJsonIO::readJsonFile(filePath, doc)) {
			throw std::exception("Corrupted File. ");
		}
		QJsonObject obj = doc.object();
		QJsonArray patternList;
		if (obj.contains("patternList"))
		{
			patternList= obj["patternList"].toArray();
			QMap<QString, FileMappingPattern>patternMap;
			for (auto iter = patternList.begin(); iter != patternList.end(); iter++) {
				QJsonObject patternObj = iter->toObject();
				FileMappingPattern pattern(patternObj);
				patternMap.insert(pattern.name(), pattern);
			}
			std::lock_guard<std::mutex>locker(mutex_fileMappingPatern);
			map_fileMappingPattern = patternMap;
		}
		currentRuleFilePath = filePath;
		GlobalSettings::Instance().latestRuleFilePath = filePath;
		emit sig_fileMappingManager_ruleFileOpened();
		emit sig_fileMappingManager_patternUpdated();
	}
	catch (std::exception e) {
		QString msg = "Fail Open File. ";
		GlobalMessageRepost::Instance().sendNewMsg(msg + e.what());
		return false;
	}
	
	return true;
}
void FileMappingManager::closeCurrRuleFile()
{
	std::lock_guard<std::mutex>locker(mutex_fileMappingPatern);
	map_fileMappingPattern.clear();
	currentRuleFilePath.clear();
	emit sig_fileMappingManager_patternUpdated();
}
void FileMappingManager::printRuleFileToConsole()
{
	std::lock_guard<std::mutex>locker(mutex_fileMappingPatern);
	QString stringOut;
	for (auto iter = map_fileMappingPattern.begin(); iter != map_fileMappingPattern.end(); iter++) {
		stringOut+=iter->toConsoleString();
	}
	printf_s("\n");
	printf_s(stringOut.toStdString().c_str());
}
QString FileMappingManager::getCurrentRuleFilePath()
{
	return currentRuleFilePath;
}
bool FileMappingManager::saveRuleFile(QString filePath)
{
	try {
		std::lock_guard<std::mutex>locker(mutex_fileMappingPatern);
		QJsonDocument doc;
		QJsonObject obj;
		QJsonArray patternList;
		for (auto iter = map_fileMappingPattern.begin(); iter != map_fileMappingPattern.end(); iter++) {
			patternList.append(iter->toJsonObj());
		}
		obj.insert("patternList", patternList);
		doc.setObject(obj);
		if (!QJsonIO::writeJsonFile(filePath, doc)) {
			throw std::exception(QString("Unable to write file\n×["+filePath+"]").toStdString().c_str());
		}
		QString msg = "File Saved.";
		if (currentRuleFilePath != filePath) {
			msg += "\nFile Path [" + filePath + "]";
		}
		GlobalMessageRepost::Instance().sendNewMsg(msg);
		currentRuleFilePath = filePath;
		emit sig_fileMappingManager_ruleFileOpened();
		return true;
	}
	catch (std::exception e) {
		QString msg = "Fail Save File. ";
		GlobalMessageRepost::Instance().sendNewMsg(msg + e.what());
		return false;
	}
}

void FileMappingManager::rec_deletePattern(QString name)
{
	deletePattern(name);
}
void FileMappingManager::rec_applyPattern(QString  patternName)
{
	applyPattern(patternName);
}
void FileMappingManager::rec_printPatternsToConsole()
{
	printRuleFileToConsole();
}
bool FileMappingManager::createNewPattern(QString patternName)
{
	try {
		if (patternName.isEmpty()) {
			throw std::exception("Pattern Name is Empty");
		}
		auto iter = map_fileMappingPattern.find(patternName);
		if (iter != map_fileMappingPattern.end()) {
			throw std::exception("Pattern already Exist");
		}
		else {
			map_fileMappingPattern[patternName] = FileMappingPattern(patternName);
		}
		QString msg = "Create New Pattern [" + patternName + "].";
		GlobalMessageRepost::Instance().sendNewMsg(msg, GlobalMessageRepost::MsgDst::MainWindowUserMsgBrowser);
		emit sig_fileMappingManager_patternUpdated();
		return true;
	}
	catch (std::exception e) {
		QString msg = "Fail Create New Pattern [" + patternName + "]."+e.what();
		GlobalMessageRepost::Instance().sendNewMsg(msg, GlobalMessageRepost::MsgDst::MainWindowUserMsgBrowser);
		return false;
	}
}

bool FileMappingManager::deletePattern(QString patternName)
{
	try {
		if (patternName.isEmpty()) {
			throw std::exception("Pattern Name is Empty");
		}
		if (!map_fileMappingPattern.contains(patternName)) {
			throw std::exception("Pattern already Exist");
		}
		else {
			map_fileMappingPattern.remove(patternName);
		}
		QString msg = "Delete Pattern [" + patternName + "].";
		GlobalMessageRepost::Instance().sendNewMsg(msg, GlobalMessageRepost::MsgDst::MainWindowUserMsgBrowser);
		emit sig_fileMappingManager_patternUpdated();
		return true;
	}
	catch (std::exception e) {
		QString msg = "Fail Delete Pattern [" + patternName + "]." + e.what();
		GlobalMessageRepost::Instance().sendNewMsg(msg, GlobalMessageRepost::MsgDst::MainWindowUserMsgBrowser);
		return false;
	}
}
bool FileMappingManager::applyPattern(QString)
{
	return false;
}

void FileMappingManager::rec_openRuleFile(QString path) {
	openRuleFile(path);
}

void FileMappingManager::rec_closeCurrRuleFile()
{
	closeCurrRuleFile();
}

void FileMappingManager::rec_saveRuleFile(QString path)
{
	saveRuleFile(path);
}

void FileMappingManager::rec_createNewPattern(QString patternName) {
	createNewPattern(patternName);
}
