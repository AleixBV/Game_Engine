#ifndef __ComponentTransform_H__
#define __ComponentTransform_H__

#include "Globals.h"
#include "Component.h"
#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"

class ComponentTransform : public Component
{
public:
	ALIGN_CLASS_TO_16

	//Constructor
	ComponentTransform(float3 &position, float3 &scale, Quat &rot);

	//Destructor
	~ComponentTransform();

	float3 position;
	float3 scale;
	Quat rot = Quat::identity;
	float3 euler_rot;
};

#endif