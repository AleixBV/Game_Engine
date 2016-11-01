#ifndef __Component_H__
#define __Component_H__

enum ComponentType
{
	TRANSFORMATION_COMPONENT,
	MESH_COMPONENT,
	MATERIAL_COMPONENT
};

class Component
{
public:
	//Constructor
	Component();

	//Destructor
	virtual ~Component();

	ComponentType type;

	virtual void Enable();
	virtual void Update();
	virtual void Disable();

	bool active;
};

#endif