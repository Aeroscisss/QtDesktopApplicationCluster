#include <QObject>
#include <QMap>
#include <QString>
#include <QStringList>
#include <memory>
#include <mutex>
#include "FileSyncPattern.h"
class FileSyncManager:public QObject
{
	Q_OBJECT
public:
	static FileSyncManager& Instance();
	~FileSyncManager();
	[[nodisacard]]QString newPatternNameSuggestion();
	bool getPattern(QString PatternName,FileSyncPattern&);
	QStringList getPatternNames();
	bool updatePattern(QString PatternName,FileSyncPattern &Pattern);
	bool saveRuleFile(QString filePath);
	bool openRuleFile(QString filePath);
	void closeCurrRuleFile();
	void printRuleFileToConsole();
	QString currRuleFilePath();
	QString getCurrentRuleFilePath();
public slots:
	void rec_openRuleFile(QString);
	void rec_closeCurrRuleFile();
	void rec_saveRuleFile(QString);
	void rec_createNewPattern(QString);
	void rec_deletePattern(QString);
	void rec_applyPattern(QString);
	void rec_printPatternsToConsole();
signals:
	void sig_fileSyncManager_patternUpdated();
	void sig_fileSyncManager_ruleFileOpened();
	void sig_fileSyncManager_ruleFileClosed();
protected:
private:
	FileSyncManager();
	bool createNewPattern(QString);
	bool deletePattern(QString);
	bool applyPattern(QString);
private:
	QMap<QString, FileSyncPattern>map_fileSyncPattern;
	QString currentRuleFilePath;
};