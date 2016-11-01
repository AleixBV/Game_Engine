#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(unsigned int material_id) : material_id(material_id)
{
	type = MATERIAL_COMPONENT;
}

ComponentMaterial::~ComponentMaterial()
{
}