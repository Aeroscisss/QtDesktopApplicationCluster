#pragma once
#include "FileMappingRule.h"
#include <QChar>
class FileMappingRuleParser
{
public:
	static FileMappingRuleParser& Instance();
	~FileMappingRuleParser();
	static FileMappingRule parseRule(QString ruleString);
private:
	FileMappingRuleParser();
	static QStringList parseUnit(QString header, QChar startSignChar, QChar endSignChar, QString target);
};

