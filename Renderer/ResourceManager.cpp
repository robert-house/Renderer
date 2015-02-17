#include "ResourceManager.h"

ResourceManager::ResourceManager() {}
ResourceManager::~ResourceManager() {}

bool ResourceManager::Push()
{
	//unsigned int hash = Hash(r.getName);
	//r.setID(hash);

	//_resTable.emplace(hash, r);

	return true;
}

void ResourceManager::Find(unsigned int id)
{
	//return &_resTable.at(id);
}

unsigned int ResourceManager::Hash(string s)
{
	// insert hashing code here

	return 1;
}