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
	bool ret = false;
	std::vector<Component*>::const_iterator iterator = components.begin();
	while (iterator != components.end())
	{
		if ((*iterator)->type == type)
		{
			components_to_return->push_back(*iterator);
			ret = true;
		}

		iterator++;
	}

	return ret;
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

float4x4 GameObject::GetLocalMatrix() const
{
	return local_matrix;
}

void GameObject::SetTRS(const float3& new_position, const Quat& new_rot, const float3& new_scale)
{
	std::vector<Component*> components_transformation;
	if (FindComponent(&components_transformation, TRANSFORMATION_COMPONENT))
	{
		ComponentTransform* transformation = (ComponentTransform*)components_transformation[0];
		transformation->position = new_position;
		transformation->rot = new_rot;
		transformation->scale = new_scale;
		local_matrix = float4x4::FromTRS(new_position, new_rot, new_scale);
	}

}

void GameObject::SetPosition(const float3& new_position)
{
	std::vector<Component*> components_transformation;
	if (FindComponent(&components_transformation, TRANSFORMATION_COMPONENT))
	{
		ComponentTransform* transformation = (ComponentTransform*)components_transformation[0];
		transformation->position = new_position;
		local_matrix = float4x4::FromTRS(transformation->position, transformation->rot, transformation->scale);
	}
}

void GameObject::SetRotation(const Quat& new_rotation)
{
	std::vector<Component*> components_transformation;
	if (FindComponent(&components_transformation, TRANSFORMATION_COMPONENT))
	{
		ComponentTransform* transformation = (ComponentTransform*)components_transformation[0];
		transformation->rot = new_rotation;
		local_matrix = float4x4::FromTRS(transformation->position, transformation->rot, transformation->scale);
	}
}

void GameObject::SetScale(const float3& new_scale)
{
	std::vector<Component*> components_transformation;
	if (FindComponent(&components_transformation, TRANSFORMATION_COMPONENT))
	{
		ComponentTransform* transformation = (ComponentTransform*)components_transformation[0];
		transformation->scale = new_scale;
		local_matrix = float4x4::FromTRS(transformation->position, transformation->rot, transformation->scale);
	}
}