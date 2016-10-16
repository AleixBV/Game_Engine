#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{

}

bool GameObject::FindComponent(std::vector<Component*>* components_to_return, ComponentType type)
{
	bool ret = true;
	std::vector<Component*>::iterator iterator = components.begin();
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
