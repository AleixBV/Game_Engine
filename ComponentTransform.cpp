#include "ComponentTransform.h"
#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"

ComponentTransform::ComponentTransform(float3 &position, float3 &scale, Quat &rot) : position(position), scale(scale), rot(rot)
{
	type = TRANSFORMATION_COMPONENT;
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Update()
{
}