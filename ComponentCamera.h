#ifndef __ComponentCamera_H__
#define __ComponentCamera_H__

#include "Component.h"
#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"

class ComponentCamera : public Component
{
public:
	//Constructor
	ComponentCamera();

	//Destructor
	~ComponentCamera();

	void Update(GameObject* parent);

	Frustum camera;
	bool frustum_culling_active;
	bool form_changed;
	bool position_changed;
};

#endif