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
public slots:
	void rec_createNewPattern(QString);
	void rec_deletePattern(QString);
signals:
	void sig_fileMappingManager_patternUpdated();
protected:
private:
	FileMappingManager();
	bool createNewPattern(QString);
	bool deletePattern(QString);
private:
	QMap<QString, FileMappingPattern>map_fileMappingPattern;
	std::mutex mutex_fileMappingPatern;
};