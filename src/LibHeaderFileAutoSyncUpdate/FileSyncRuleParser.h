#pragma once
#include "FileSyncRule.h"
#include <QChar>
class FileSyncRuleParser
{
public:
	static FileSyncRuleParser& Instance();
	~FileSyncRuleParser();
	static FileSyncRule parseRule(QString ruleString);
private:
	FileSyncRuleParser();
	static QStringList parseUnit(QString header, QChar startSignChar, QChar endSignChar, QString target);
};

