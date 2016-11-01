#ifndef __ComponentMaterial_H__
#define __ComponentMaterial_H__

#include "Component.h"

class ComponentMaterial : public Component
{
public:
	//Constructor
	ComponentMaterial(unsigned int material_id);

	//Destructor
	~ComponentMaterial();

	unsigned int material_id;
};

#endif