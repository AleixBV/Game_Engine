#include "ComponentTransform.h"
#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"

ComponentTransform::ComponentTransform(float3 &position, float3 &scale, Quat &rot) : position(position), scale(scale), rot(rot)
{
	type = TRANSFORMATION_COMPONENT;
	local_matrix = float4x4::FromTRS(position, rot, scale);
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Update()
{
}

float4x4 ComponentTransform::GetLocalMatrix() const
{
	return local_matrix;
}