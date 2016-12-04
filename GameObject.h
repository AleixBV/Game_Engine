#ifndef __GameObject_H__
#define __GameObject_H__

#include "Globals.h"
#include <vector>
#include "Component.h"
#include <string>
#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"

class Component;

class GameObject
{
	friend class Component;

public:
	enum WireframeTypeDraw
	{
		WIREFRAME_NORMAL_DRAW = 0,
		WIREFRAME_SELECTED_DRAW,
		WIREFRAME_PARENT_SELECTED_DRAW
	};

	ALIGN_CLASS_TO_16

	//Constructor
	GameObject(GameObject* parent = nullptr, const char* name = "Unnamed");

	//Destructor
	~GameObject();

	GameObject* parent = nullptr;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> children;
	WireframeTypeDraw type_draw;
	AABB bbox;
	OBB actual_bbox;
	bool bbox_changed = false;
	bool visible_frustum = true;//is visible by frustum from camera?

	void Update();
	bool FindComponent(std::vector<Component*>* components, Component::ComponentType type) const;
	bool GetLocalPosition(float3* pos) const;
	bool GetGlobalPosition(float3* pos) const;
	float4x4 GetGlobalTransform() const;
	float4x4 GetLocalMatrix() const;

	void SetTRS(const float3& new_position, const Quat& new_rot, const float3& new_scale);
	void SetPosition(const float3& new_position);
	void SetRotation(const Quat& new_rotation);
	void SetRotation(const float3& new_rotation);
	void SetScale(const float3& new_scale);
	void RecursivelyCalculateBbox();
	void RecalculateBbox();

private:
	float4x4 local_matrix;

	bool GetGlobalTransform(float4x4& transform) const;
};

#endif