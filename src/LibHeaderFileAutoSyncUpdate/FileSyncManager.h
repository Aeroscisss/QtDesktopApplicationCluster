#include <QObject>
#include <QMap>
#include <QString>
#include <QStringList>
#include <memory>
#include <mutex>
#include "CustomQToolKit/ImprovedThreadBase.h"
#include "FileSyncPattern.h"
class FileSyncManager:public QObject
{
	Q_OBJECT
public:
	static FileSyncManager& Instance();
	ImprovedThreadBase m_thread;
	~FileSyncManager();
	[[nodisacard]]QString newPatternNameSuggestion();
	bool getPattern(QString PatternName,FileSyncPattern&);
	bool updatePattern(QString patternName, FileSyncPattern& pattern);
	[[nodisacard]] QStringList getPatternNames();
	[[nodisacard]] QString getCurrentRuleFilePath();
public slots:
	void rec_openRuleFile(QString);
	void rec_saveRuleFile(QString path);
	void rec_closeCurrRuleFile();
	void rec_createNewPattern(QString);
	void rec_deletePattern(QString);
	void rec_printPatternsToConsole();
signals:
	void sig_fileSyncManager_patternUpdated();
	void sig_fileSyncManager_ruleFileOpened();
	void sig_fileSyncManager_ruleFileClosed();
protected:
private:
	FileSyncManager();
	bool openRuleFile(QString filePath);
	bool saveRuleFile(QString filePath);
	void closeCurrRuleFile();
	bool createNewPattern(QString);
	bool deletePattern(QString patternName);
	void printRuleFileToConsole();
private:
	friend class CustomQApp;
	std::mutex mutex_patterns;
	QMap<QString, FileSyncPattern>map_fileSyncPattern;
	QString currentRuleFilePath;
};