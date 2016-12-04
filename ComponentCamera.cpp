#include "ComponentCamera.h"
#include "GameObject.h"

ComponentCamera::ComponentCamera()
{
	type = CAMERA_COMPONENT;
	frustum_culling_active = true;
	form_changed = false;
	position_changed = false;

	camera.type = FrustumType::PerspectiveFrustum;
	camera.pos = float3(0.0f, 0.0f, 0.0f);
	camera.front = float3(0.0f, 0.0f, 1.0f);
	camera.up = float3(0.0f, 1.0f, 0.0f);
	camera.nearPlaneDistance = 0.5f;
	camera.farPlaneDistance = 100.0f;
	camera.verticalFov = DegToRad(60.0f);
	camera.horizontalFov = 2.0f * atanf(tanf(camera.verticalFov / 2.0f) * (16.0f / 9.0f));
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Update(GameObject* parent)
{
	float4x4 pos = parent->GetGlobalTransform();
	if (!pos.TranslatePart().Equals(camera.pos))
	{
		camera.pos = pos.TranslatePart();
		camera.front = pos.WorldZ();
		camera.up = pos.WorldY();
	}
}