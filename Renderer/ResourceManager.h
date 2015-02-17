#pragma once

#include <unordered_map>
#include <string>
#include "Resource.h"

using namespace std;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
	bool Push();
	void Find(unsigned int ID);

private:
	//unordered_map<unsigned int, Resource> _resTable;
	unsigned int Hash(string s);
};