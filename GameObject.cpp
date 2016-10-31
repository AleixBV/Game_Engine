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

bool GameObject::GetLocalPosition(float3* pos) const
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

bool GameObject::GetGlobalPosition(float3* pos) const
{
	bool ret = false;
	std::vector<Component*> components_transformation;
	if (FindComponent(&components_transformation, TRANSFORMATION_COMPONENT))
	{
		ComponentTransform* transformation = (ComponentTransform*)components_transformation[0];
		float4x4 transform = float4x4::FromTRS(transformation->position, transformation->rot, transformation->scale);

		if (parent != nullptr)
		{
			float3 parent_pos;
			parent->GetGlobalTransform(transform);
		}

		*pos = transform.TranslatePart();

		ret = true;
	}

	return ret;
}

bool GameObject::GetGlobalTransform(float4x4& transform) const
{
	bool ret = false;
	std::vector<Component*> components_transformation;
	if (FindComponent(&components_transformation, TRANSFORMATION_COMPONENT))
	{
		ComponentTransform* transformation = (ComponentTransform*)components_transformation[0];
		transform = transform * (float4x4::FromTRS(transformation->position, transformation->rot, transformation->scale));

		if (parent != nullptr)
			parent->GetGlobalTransform(transform);


		ret = true;
	}

	return ret;
}