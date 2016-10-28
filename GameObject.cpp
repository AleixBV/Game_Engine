#include "GameObject.h"

GameObject::GameObject(GameObject* parent, const char* name) : parent(parent), name(name)
{
	debug_draw = false;
}

GameObject::~GameObject()
{
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
