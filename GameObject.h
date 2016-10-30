#ifndef __GameObject_H__
#define __GameObject_H__

#include <vector>
#include "Component.h"
#include <string>
#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"

enum WireframeTypeDraw
{
	WIREFRAME_NORMAL_DRAW = 0,
	WIREFRAME_SELECTED_DRAW,
	WIREFRAME_PARENT_SELECTED_DRAW
};

class GameObject
{
public:
	//Constructor
	GameObject(GameObject* parent = nullptr, const char* name = "unnamed");

	//Destructor
	~GameObject();

	GameObject* parent = nullptr;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> children;
	WireframeTypeDraw type_draw;

	void Update();
	bool FindComponent(std::vector<Component*>* components, ComponentType type) const;
	bool GetPosition(float3* pos);
};

#endif