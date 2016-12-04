#include "Component.h"

Component::Component()
{
	active = true;
}

Component::~Component()
{
}

void Component::Enable()
{
	active = true;
}

void Component::Update(GameObject* parent)
{
}

void Component::Disable()
{
	active = false;
}
