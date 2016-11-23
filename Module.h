#ifndef __Module_H__
#define __Module_H__

#include <string>
#include "PugiXml/src/pugixml.hpp"

class Application;
struct PhysBody3D;

class Module
{
private :
	bool enabled;

protected:
	std::string name;

public:
	Application* App;

	Module(Application* parent, bool start_enabled = true) : App(parent)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
	{
		return true; 
	}
	
	// Called before render is available
	virtual bool Awake(pugi::xml_node& config)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	std::string GetName() const
	{
		return name;
	}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}

	virtual void DebugDraw()
	{}

	virtual bool SaveConfig(pugi::xml_node&) const
	{
		return true;
	}

	virtual bool SaveGame(pugi::xml_node&) const
	{
		return true;
	}
};
#endif