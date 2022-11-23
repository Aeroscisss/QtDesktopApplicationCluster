#include <QObject>
#include<memory>
class FileMappingManager:public QObject
{
	Q_OBJECT
public:
	FileMappingManager& Instance();
	~FileMappingManager();
protected:

private:
	FileMappingManager();
private:
	
};