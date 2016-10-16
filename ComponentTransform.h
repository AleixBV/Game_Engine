#ifndef __ComponentTransform_H__
#define __ComponentTransform_H__

#include "Component.h"
#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"

class ComponentTransform : public Component
{
public:
	//Constructor
	ComponentTransform(float3 &position, float3 &scale, Quat &rot);

	//Destructor
	~ComponentTransform();

	void Update();

	float3 position;
	float3 scale;
	Quat rot = Quat::identity;
};

#endif