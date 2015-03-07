#include "Entity.h"
#include "Model.h"

class EntityDrawable : public Entity
{
public:
	EntityDrawable(string name);
	~EntityDrawable();

	virtual bool Load(LPCWSTR path);
	virtual bool Update();

	Model* getModel();
	XMFLOAT4X4 getWorldMatrix();

private:
	Model *_model;
	XMFLOAT4X4			_worldMatrix;

	bool BuildWorldMatrix();

};