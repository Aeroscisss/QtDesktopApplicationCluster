#include "FileMappingManager.h"
FileMappingManager & FileMappingManager::Instance() {
	static std::unique_ptr<FileMappingManager>instance_ptr =
		std::make_unique<FileMappingManager>();
	return *instance_ptr;
}
FileMappingManager::~FileMappingManager() {

}
FileMappingManager::FileMappingManager() {

}
