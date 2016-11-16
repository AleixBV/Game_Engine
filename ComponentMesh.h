#ifndef __Mesh_H__
#define __Mesh_H__

#include "Globals.h"
#include "Component.h"

struct ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	const char* name = "Unnamed";

	uint id_vertices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_indices = 0; // id in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;

	uint id_normals = 0; //id in VRAM
	float* normals = nullptr;
	uint id_colors = 0; //id in VRAM
	float* colors = nullptr;
	uint id_texture_coordinates = 0; //id in VRAM
	float* texture_coordinates = nullptr;
};

#endif