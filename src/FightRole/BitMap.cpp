#include "BitMap.h"
#include "string.h"
#include <assert.h>
BitMap::BitMap(int startLimit , int endLimit):
startLimit_(startLimit),
endLimit_(endLimit)
{
	bitMapData_ = new bool [endLimit_ - startLimit_ + 1];
	memset(bitMapData_ , 0 , endLimit_ - startLimit_ + 1);
}

BitMap::~BitMap(void)
{
	try
	{
		if (bitMapData_)
		{
			delete [] bitMapData_;
			bitMapData_ = NULL;
		}
	}
	catch (...)
	{
	}
}

bool BitMap::setBitValue(int bitIndex , bool bitValue){
	if (bitIndex < startLimit_ || bitIndex > endLimit_)
	{
		return false ;
	}
	bitMapData_[bitIndex - startLimit_] = bitValue;
	return true;
}
bool BitMap::getBitValue(int bitIndex){
	if (bitIndex < startLimit_ || bitIndex > endLimit_)
	{
		return true;
	}
	return bitMapData_[bitIndex - startLimit_];
}
int  BitMap::getFreeBit(void){
	for (int i = 0 ; i <= endLimit_ - startLimit_ ;i++)
	{
		if (!bitMapData_[i]){
			bitMapData_[i] = true;
			return i + startLimit_;
		}
	}
	assert(0 && "无法获取可使用的部队ID");
	return -1;
}