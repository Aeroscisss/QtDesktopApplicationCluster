#include "FileMappingUnit.h"

FileMappingUnit::FileMappingUnit()
{
}

FileMappingUnit::~FileMappingUnit()
{
}

bool FileMappingUnit::valid()
{
	return m_valid;
}

void FileMappingUnit::setValid(bool in_valid)
{
	m_valid = in_valid;
}
