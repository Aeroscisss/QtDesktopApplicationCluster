#include "GlobalSettings.h"
#include <memory>
#include <iostream>
#include <CustomQToolKit/QJsonIO.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace std;
GlobalSettings& GlobalSettings::Instance() {
	static std::unique_ptr<GlobalSettings>instance_ptr =
		std::unique_ptr<GlobalSettings>(new GlobalSettings());
	return *instance_ptr;
}

GlobalSettings::GlobalSettings()
{
	this->moveToThread(&thread_globalSettings);
	thread_globalSettings.start();
}

GlobalSettings::~GlobalSettings()
{
	thread_globalSettings.requestInterruption();
	std::this_thread::sleep_for(chrono::milliseconds(10));
	thread_globalSettings.quit();
	thread_globalSettings.wait();
}

void GlobalSettings::outputSettingFile()
{
	QJsonDocument doc;
	QJsonObject settingItems;
	QJsonObject userItems;
	QJsonObject enternalItems;
	QJsonObject devItems;
	userItems.insert("reletiveLatestRuleFilePath", reletiveLatestRuleFilePath);
	userItems.insert("absoluteLatestRuleFilePath", absoluteLatestRuleFilePath);
	settingItems["userItems"]=userItems;
	doc.setObject(settingItems);
	QJsonIO::writeJsonFile("./config/settings.json",doc);
}

void GlobalSettings::readSettingFile()
{
	QJsonDocument doc;
	QJsonIO::readJsonFile("./config/settings.json", doc);
	QJsonObject settingItems = doc.object();
	if (settingItems.contains("userItems")) {
		QJsonObject userItems=settingItems["userItems"].toObject();
		std::lock_guard<std::mutex>locer(mutex_settingItem);
		if (userItems.contains("reletiveLatestRuleFilePath")) {
			reletiveLatestRuleFilePath = userItems["reletiveLatestRuleFilePath"].toString();
		}
		if (userItems.contains("absoluteLatestRuleFilePath")) {
			absoluteLatestRuleFilePath = userItems["absoluteLatestRuleFilePath"].toString();
		}
	}
	//QJsonObject enternalItems;
	//QJsonObject devItems;
}
