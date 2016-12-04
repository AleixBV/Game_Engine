#ifndef __Component_H__
#define __Component_H__

class GameObject;

class Component
{
public:

	enum ComponentType
	{
		TRANSFORMATION_COMPONENT,
		MESH_COMPONENT,
		MATERIAL_COMPONENT,
		CAMERA_COMPONENT
	};

	//Constructor
	Component();

	//Destructor
	virtual ~Component();

	ComponentType type;

	virtual void Enable();
	virtual void Update(GameObject* parent);
	virtual void Disable();

	bool active;
};

#endif