#include "FileSyncOperator.h"
#include <memory>
#include <QCryptographicHash>
#include <QTextStream>
#include <QDir>
#include "QFileInfo"
#include "GlobalMessageRepost.h"
FileSyncOperator& FileSyncOperator::Instance()
{
	static std::unique_ptr<FileSyncOperator>instance_ptr=std::unique_ptr<FileSyncOperator>(new FileSyncOperator);
	return *instance_ptr;
}

FileSyncOperator::FileSyncOperator()
{
	threadIsInterrupted = true;
	ruleQueue = std::make_unique<MultiThreadQueue<FileSyncRule>>();
}

FileSyncOperator::~FileSyncOperator()
{
	threadIsInterrupted = true;
	if(m_thread.joinable())
		m_thread.join();
	GlobalMessageRepost::Instance().sendNewMsg("FileSyncOperator is deleted",1);
}

void FileSyncOperator::threadLoopRun()
{
	if (threadIsInterrupted == true) {
		threadIsInterrupted = false;
		m_thread = std::thread(&ruleOpLoop);
	}
}

void FileSyncOperator::ruleOpLoop()
{
	FileSyncRule rule;
	while (!FileSyncOperator::Instance().threadIsInterrupted)
	{
		if (FileSyncOperator::Instance().ruleQueue->poll(rule)) {
			GlobalMessageRepost::Instance().sendNewMsg("try apply new rule", 1);
			FileSyncOperator::Instance().applyRule(rule);
			GlobalMessageRepost::Instance().sendNewMsg("new rule applied", 1);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
}

void FileSyncOperator::applyRule(FileSyncRule rule)
{
	QStringList filters = rule.filterList();//获取正则匹配列表
	QStringList srcs = rule.srcList();//获取源值列表
	QMap<QString, CustomFileInfo>srcFileMap;//源文件Map
	convertPathValueIntoFileInfo(srcs, srcFileMap);
	regexCheck(srcFileMap, filters);
	QString dst = rule.dst();//获取目的地值列表
	QMap<QString, CustomFileInfo>dstFileMap;//目的地文件Map
	if (QFileInfo(dst).isDir()) {
		findAllFilesUnderFolder(dst, dstFileMap);
		if (!dst.isEmpty()) {
			QChar lastChar = *(dst.end() - 1);
			if (lastChar != '/' && lastChar != '\\')
				dst += '/';
		}
	}
	for (auto srcFileInfo : srcFileMap) {
		auto iter = dstFileMap.find(srcFileInfo.fileCompleteName);
		if (iter != dstFileMap.end()) {//确实存在
			CustomFileInfo dstFileInfo = iter.value();
			if (srcFileInfo.fileByteSize < 1024 * 1024 * 10) {//小于10MB
				srcFileInfo.md5 = smallFileMd5(srcFileInfo.filePath);
				dstFileInfo.md5 = smallFileMd5(dstFileInfo.filePath);
			}
			else {
				srcFileInfo.md5 = bigFileMd5(srcFileInfo.filePath);
				dstFileInfo.md5 = bigFileMd5(dstFileInfo.filePath);
			}
			if (srcFileInfo.md5 != dstFileInfo.md5) {//文件有改变
				//移动src到dst
				copyFile(srcFileInfo.filePath, dst + srcFileInfo.fileCompleteName,true);
			}
		}
		else {//根本不存在
			copyFile(srcFileInfo.filePath, dst+srcFileInfo.fileCompleteName, true);
		}
	}
	GlobalMessageRepost::Instance().sendNewMsg("Apply Rule Finished");
}

void FileSyncOperator::convertPathValueIntoFileInfo(QStringList files, QMap<QString, CustomFileInfo>& fileInfoMap)
{
	QMap<QString, CustomFileInfo> newMap;
	QFileInfo fileInfo;
	for (auto value : files) {
		fileInfo= QFileInfo(value);
		if (!fileInfo.exists()) {//不存在，跳过
			continue;
		}
		if (fileInfo.isFile()) {
			CustomFileInfo customFileInfo=CustomFileInfo(value);
			customFileInfo.isFileFlag = true;
			customFileInfo.fileCompleteName = fileInfo.completeBaseName() + "." + fileInfo.suffix();
			customFileInfo.fileBaseName = fileInfo.completeBaseName();
			customFileInfo.fileByteSize = fileInfo.size();
			newMap.insert(customFileInfo.fileCompleteName, customFileInfo);
			continue;
		}
		if (fileInfo.isDir()) {
			findAllFilesUnderFolder(value, fileInfoMap);
			continue;
		}
	}
}

void FileSyncOperator::findAllFilesUnderFolder(QString folderPath, QMap<QString, CustomFileInfo>& fileInfoMap)
{
		QDir dir(folderPath);
	QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	for (auto fileInfo:file_list) {
		CustomFileInfo customFileInfo = CustomFileInfo(fileInfo.filePath());
		customFileInfo.isFileFlag = true;
		customFileInfo.fileCompleteName = fileInfo.completeBaseName()+"."+ fileInfo.suffix();
		customFileInfo.fileBaseName = fileInfo.completeBaseName();
		customFileInfo.fileByteSize = fileInfo.size();
		fileInfoMap.insert(customFileInfo.fileCompleteName, customFileInfo);
		continue;
	}
	QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
	for (int i = 0; i != folder_list.size(); i++)
	{
		QString folderName = folder_list.at(i).absoluteFilePath();
		findAllFilesUnderFolder(folderName,fileInfoMap);
	}
}



QString FileSyncOperator::smallFileMd5(QString path)
{
	QFile file(path);
	QTextStream in(&file);
	QString str;
	QByteArray bb;
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		str = in.readAll();
		//qDebug() << str;
		bb = QCryptographicHash::hash(str.toLatin1(), QCryptographicHash::Md5);
		str = bb.toHex();
	}
	file.close();
	return  str;
}

QString FileSyncOperator::bigFileMd5(QString path)
{
	QString str;
	QFile localFile(path);

	if (!localFile.open(QIODevice::ReadOnly))
	{
		//qDebug() << "file open error.";
		return QString();
	}

	QCryptographicHash ch(QCryptographicHash::Md5);

	quint64 totalBytes = 0;
	quint64 bytesWritten = 0;
	quint64 bytesToWrite = 0;
	quint64 loadSize = 1024 * 4;
	QByteArray buf;

	totalBytes = localFile.size();
	bytesToWrite = totalBytes;

	while (1){
		if (bytesToWrite > 0){
			buf = localFile.read(qMin(bytesToWrite, loadSize));
			ch.addData(buf);
			bytesWritten += buf.length();
			bytesToWrite -= buf.length();
			buf.resize(0);
		}
		else{
			break;
		}
		if (bytesWritten == totalBytes){
			break;
		}
	}
	localFile.close();
	QByteArray md5 = ch.result();
	str = md5.toHex();
	//qDebug() << md5.toHex();
	return  str;
}

void FileSyncOperator::regexCheck(QMap<QString, CustomFileInfo>& map, QStringList regexList)
{
	QMap<QString, CustomFileInfo> newMap;
	for (auto iter = map.begin(); iter != map.end();iter++) {
		QRegExp regExp;
		for (auto reg : regexList) {
			regExp.setPattern(reg);
			if(!reg.isValidUtf16())
				continue;
			if (regExp.indexIn(iter.key()) >= 0) {//匹配ok
				newMap.insert(iter.key(),iter.value());
			}
		}
	}
}

bool FileSyncOperator::copyFile(QString srcPath, QString dstPath, bool coverFileIfExist)
{
	srcPath.replace("\\", "/");
	dstPath.replace("\\", "/");
	if (srcPath == dstPath) {
		return true;
	}
	if (!QFile::exists(srcPath)) {  //源文件不存在
		QString errMsg = "File[" + srcPath + "] does not exist";
		GlobalMessageRepost::Instance().sendNewMsg(errMsg);
		return false;
	}
	if (QFile::exists(dstPath)) {
		if (coverFileIfExist) {
			QFile::remove(dstPath);
		}
	}
	if (!QFile::copy(srcPath, dstPath)) {
		QString errMsg = "File[" + srcPath + "] copy fail";
		GlobalMessageRepost::Instance().sendNewMsg(errMsg);
		return false;
	}
	QString msg = "File[" + srcPath + "] copy success";
	GlobalMessageRepost::Instance().sendNewMsg(msg);
	return true;
}


