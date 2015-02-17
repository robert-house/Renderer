#pragma once

#include <string>
#include "SimpleMath.h"

using namespace std;

class Resource
{
public:
	Resource();
	virtual ~Resource();
	virtual bool LoadFromFile(string filePath);
	virtual bool Release();
	string getName();
	unsigned int getID();
	unsigned int setID(unsigned int id);

protected:
	string _name;
	unsigned int _ID;
	unsigned int _count;
};