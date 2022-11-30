#include "FileMappingManager.h"
#include "FileMappingPattern.h"
#include "GlobalMessageRepost.h"
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
		GlobalMessageRepost::Instance().sendNewMsg("Update Patterm[" + patternName + "]. ",1);
		return true;
	}
	catch(std::exception e) {
		QString msg = "Fail Update Patterm[" + patternName + "]. ";
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

void FileMappingManager::rec_createNewPattern(QString patternName) {
	createNewPattern(patternName);
}
