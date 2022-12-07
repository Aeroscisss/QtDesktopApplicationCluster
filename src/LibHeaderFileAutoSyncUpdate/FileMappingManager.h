#include <QObject>
#include <QMap>
#include <QString>
#include <QStringList>
#include <memory>
#include <mutex>
#include "FileMappingPattern.h"
class FileMappingManager:public QObject
{
	Q_OBJECT
public:
	static FileMappingManager& Instance();
	~FileMappingManager();
	[[nodisacard]]QString newPatternNameSuggestion();
	bool getPattern(QString PatternName,FileMappingPattern&);
	QStringList getPatternNames();
	bool updatePattern(QString PatternName,FileMappingPattern &Pattern);
	bool saveRuleFile(QString filePath);
	bool openRuleFile(QString filePath);
	QString getCurrentRuleFilePath();
public slots:
	void rec_openRuleFile(QString);
	void rec_saveRuleFile(QString);
	void rec_createNewPattern(QString);
	void rec_deletePattern(QString);
signals:
	void sig_fileMappingManager_patternUpdated();
	void sig_fileMappingManager_ruleFileOpened();
protected:
private:
	FileMappingManager();
	bool createNewPattern(QString);
	bool deletePattern(QString);
private:
	QMap<QString, FileMappingPattern>map_fileMappingPattern;
	std::mutex mutex_fileMappingPatern;
	QString currentRuleFilePath;
};