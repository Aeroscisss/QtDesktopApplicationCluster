#include "FileMappingManager.h"
#include "FileMappingPattern.h"
#include "GlobalMessageRepost.h"
#include <QJsonDocument>
#include <QJsonArray>
#include "CustomQToolKit/QJsonIO.h"
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
		std::lock_guard<std::mutex>locker(mutex_fileMappingPatern);
		QJsonDocument doc;
		if (!QJsonIO::readJsonFile(filePath, doc)) {
			throw std::exception("Currupted File. ");
		}
		currentRuleFilePath = filePath;
		emit sig_fileMappingManager_ruleFileOpened();
	}
	catch (std::exception e) {
		QString msg = "Fail Open File. ";
		GlobalMessageRepost::Instance().sendNewMsg(msg + e.what());
		return false;
	}
	return true;
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
void FileMappingManager::rec_openRuleFile(QString path) {
	openRuleFile(path);
}

void FileMappingManager::rec_saveRuleFile(QString path)
{
	saveRuleFile(path);
}

void FileMappingManager::rec_createNewPattern(QString patternName) {
	createNewPattern(patternName);
}
