#pragma once

#include <vector>

using namespace std;

class World
{
public:
	World();
	~World();

	bool Load(string s);
	bool Update();

private:
	vector<Entity*> _entityList;

};