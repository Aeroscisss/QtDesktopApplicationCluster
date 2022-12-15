#include "FileSyncRuleParser.h"
#include <memory>
FileSyncRuleParser& FileSyncRuleParser::Instance() {
	static std::unique_ptr<FileSyncRuleParser>instance_ptr =
		std::unique_ptr<FileSyncRuleParser>(new FileSyncRuleParser);
	return *instance_ptr;
}
FileSyncRuleParser::FileSyncRuleParser() {

}
FileSyncRuleParser::~FileSyncRuleParser() {

}
QStringList FileSyncRuleParser::parseUnit(QString header, QChar startSignChar, QChar endSignChar, QString target)
{
	QStringList pathList;
	int headerPos = target.indexOf(header);//找到开头
	if (headerPos>=0) {//存在header
		bool beginOfSinglePathPassed = false;//标志位置，可以开始录入
		QString singlePath;//单个filePath
		for (int index = headerPos + header.size(); index < target.size(); index++) {//整个循环
			QChar c = target[index];
			if (c== startSignChar) {//到达开头
				continue;
			}
			if (c == endSignChar) {//到达结尾
				break;
			}
			if (c == '\"') {//是单个path的开头或者结尾
				if (!beginOfSinglePathPassed) {//是单个的开头
					beginOfSinglePathPassed = true;
					continue;//继续循环
				}
				else {//是结尾
					beginOfSinglePathPassed = false;
					pathList.append(singlePath);//添加src
					singlePath.clear();//清空当前
				}
			}
			if (beginOfSinglePathPassed) {
				singlePath.append(target[index]);//添加当前char
			}
		}
	}
	return pathList;
}

FileSyncRule FileSyncRuleParser::parseRule(QString ruleString)
{
	//from:{\".testFile/src\"}\nto:{\".testFile/dst\"}\nfilter:{}
	FileSyncRule rule;
	rule.setSrcList(parseUnit("from:", '{', '}', ruleString));
	QStringList dsts = parseUnit("to:", '{', '}', ruleString);
	if(!dsts.empty())
		rule.setDst(dsts[0]);
	rule.setFilterList(parseUnit("filter:", '{', '}', ruleString));
	return rule;
}
