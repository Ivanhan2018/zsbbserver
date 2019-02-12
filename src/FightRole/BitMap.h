#pragma once
#include "globalmacrodef.h"
class BitMap
{
public:
	BitMap(int startLimit , int endLimit);
public:
	~BitMap(void);
public:
	bool clearBit(int bitValue);
	bool setBitValue(int bitIndex , bool bitValue);
	bool getBitValue(int bitIndex);
	int getFreeBit(void);
private:
	bool * bitMapData_;
	int startLimit_;
	int endLimit_;
private:
	NO_COPYABLE_AND_ASSIGN(BitMap);
};
