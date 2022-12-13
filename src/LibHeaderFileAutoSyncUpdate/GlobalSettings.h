#pragma once

#include <QObject>
#include <QThread>
#include <mutex>
class GlobalSettings  : public QObject
{
	Q_OBJECT

public:
	static GlobalSettings& Instance();
	~GlobalSettings();
	void outputSettingFile();
	void readSettingFile();
public:
	std::mutex mutex_settingItem;
	QString latestRuleFilePath;
private:
	GlobalSettings();
	QThread thread_globalSettings;
};
