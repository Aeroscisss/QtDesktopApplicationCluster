#include "FileSyncManager.h"
#include "FileSyncPattern.h"
#include "GlobalMessageRepost.h"
#include <QJsonDocument>
#include <QJsonArray>
#include "CustomQToolKit/QJsonIO.h"
#include "GlobalSettings.h"
FileSyncManager& FileSyncManager::Instance() {
	static std::unique_ptr<FileSyncManager>instance_ptr =
		std::unique_ptr<FileSyncManager>(new FileSyncManager);
	return *instance_ptr;
}
FileSyncManager::FileSyncManager() {
	this->setObjectName("FileSyncManager");
}
FileSyncManager::~FileSyncManager() {

}
QString FileSyncManager::newPatternNameSuggestion()
{
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
	auto iter = map_fileSyncPattern.find(patternName);
	if(iter==map_fileSyncPattern.end())
		return false;
	pattern = iter.value();
	return true;
}
QStringList FileSyncManager::getPatternNames()
{
	QStringList list;
	for(auto iter=map_fileSyncPattern.begin();iter!=map_fileSyncPattern.end();iter++){
		list.append(iter.key());
	}
	return list;
}
bool FileSyncManager::updatePattern(QString patternName, FileSyncPattern& pattern)
{
	try {
		auto iter = map_fileSyncPattern.find(patternName);
		if (iter == map_fileSyncPattern.end())
			throw std::exception("No Such Pattern");
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
bool FileSyncManager::openRuleFile(QString filePath)
{
	GlobalMessageRepost::Instance().sendNewMsg("openRuleFile()", 1);
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
			QMap<QString, FileSyncPattern>patternMap;
			for (auto iter = patternList.begin(); iter != patternList.end(); iter++) {
				QJsonObject patternObj = iter->toObject();
				FileSyncPattern pattern(patternObj);
				patternMap.insert(pattern.name(), pattern);
			}
			map_fileSyncPattern = patternMap;
		}
		currentRuleFilePath = filePath;
		GlobalSettings::Instance().latestRuleFilePath = filePath;
		emit sig_fileSyncManager_ruleFileOpened();
	}
	catch (std::exception e) {
		QString msg = "Fail Open File. ";
		GlobalMessageRepost::Instance().sendNewMsg(msg + e.what());
		return false;
	}
	
	return true;
}
void FileSyncManager::closeCurrRuleFile()
{
	map_fileSyncPattern.clear();
	currentRuleFilePath.clear();
	emit sig_fileSyncManager_ruleFileClosed();
}
void FileSyncManager::printRuleFileToConsole()
{
	QString stringOut;
	for (auto iter = map_fileSyncPattern.begin(); iter != map_fileSyncPattern.end(); iter++) {
		stringOut+=iter->toConsoleString();
	}
	printf_s("\n");
	printf_s(stringOut.toStdString().c_str());
}
QString FileSyncManager::currRuleFilePath()
{
	return currentRuleFilePath;
}
QString FileSyncManager::getCurrentRuleFilePath()
{
	return currentRuleFilePath;
}
bool FileSyncManager::saveRuleFile(QString filePath)
{
	GlobalMessageRepost::Instance().sendNewMsg("saveRuleFile()",1);
	try {
		QJsonDocument doc;
		QJsonObject obj;
		QJsonArray patternList;
		for (auto iter = map_fileSyncPattern.begin(); iter != map_fileSyncPattern.end(); iter++) {
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
		emit sig_fileSyncManager_ruleFileOpened();
		return true;
	}
	catch (std::exception e) {
		QString msg = "Fail Save File. ";
		GlobalMessageRepost::Instance().sendNewMsg(msg + e.what());
		return false;
	}
}

void FileSyncManager::rec_deletePattern(QString name)
{
	deletePattern(name);
}
void FileSyncManager::rec_applyPattern(QString  patternName)
{
	applyPattern(patternName);
}
void FileSyncManager::rec_printPatternsToConsole()
{
	printRuleFileToConsole();
}
bool FileSyncManager::createNewPattern(QString patternName)
{
	try {
		if (patternName.isEmpty()) {
			throw std::exception("Pattern Name is Empty");
		}
		auto iter = map_fileSyncPattern.find(patternName);
		if (iter != map_fileSyncPattern.end()) {
			throw std::exception("Pattern already Exist");
		}
		else {
			map_fileSyncPattern[patternName] = FileSyncPattern(patternName);
		}
		QString msg = "Create New Pattern [" + patternName + "].";
		GlobalMessageRepost::Instance().sendNewMsg(msg, GlobalMessageRepost::MsgDst::MainWindowUserMsgBrowser);
		emit sig_fileSyncManager_patternUpdated();
		return true;
	}
	catch (std::exception e) {
		QString msg = "Fail Create New Pattern [" + patternName + "]."+e.what();
		GlobalMessageRepost::Instance().sendNewMsg(msg, GlobalMessageRepost::MsgDst::MainWindowUserMsgBrowser);
		return false;
	}
}

bool FileSyncManager::deletePattern(QString patternName)
{
	try {
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
bool FileSyncManager::applyPattern(QString)
{
	return false;
}

void FileSyncManager::rec_openRuleFile(QString path) {
	openRuleFile(path);
}

void FileSyncManager::rec_closeCurrRuleFile()
{
	closeCurrRuleFile();
}

void FileSyncManager::rec_saveRuleFile(QString path)
{
	saveRuleFile(path);
}

void FileSyncManager::rec_createNewPattern(QString patternName) {
	createNewPattern(patternName);
}
