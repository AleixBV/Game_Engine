#ifndef __GameObject_H__
#define __GameObject_H__

#include <vector>
#include "Component.h"

class GameObject
{
public:
	//Constructor
	GameObject(GameObject* parent = nullptr, const char* name = "unnamed");

	//Destructor
	~GameObject();

	GameObject* parent = nullptr;
	const char* name = "unnamed";
	std::vector<Component*> components;
	std::vector<GameObject*> children;

	void Update();
	bool FindComponent(std::vector<Component*>* components, ComponentType type) const;
};

#endif