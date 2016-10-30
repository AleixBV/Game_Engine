#include "Globals.h"
#include "GameObject.h"
#include "ComponentTransform.h"

GameObject::GameObject(GameObject* parent, const char* name) : parent(parent), name(name)
{
	type_draw = WIREFRAME_NORMAL_DRAW;
}

GameObject::~GameObject()
{
	for (std::vector<GameObject*>::iterator i = children.begin(); i != children.end(); i++)
	{
		RELEASE((*i));
	}
	for (std::vector<Component*>::iterator i = components.begin(); i != components.end(); i++)
	{
		RELEASE((*i));
	}
}

void GameObject::Update()
{

}

bool GameObject::FindComponent(std::vector<Component*>* components_to_return, ComponentType type) const
{
	bool ret = true;
	std::vector<Component*>::const_iterator iterator = components.begin();
	while (iterator != components.end())
	{
		if ((*iterator)->type == type)
		{
			components_to_return->push_back(*iterator);
			return true;
		}

		iterator++;
	}

	return false;
}

bool GameObject::GetPosition(float3* pos)
{
	bool ret = false;
	std::vector<Component*> transformation;
	if (FindComponent(&transformation, TRANSFORMATION_COMPONENT))
	{
		*pos = ((ComponentTransform*)transformation[0])->position;
		ret = true;
	}

	return ret;
}