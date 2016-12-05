#ifndef __Mesh_H__
#define __Mesh_H__

#include "Globals.h"
#include "Component.h"
#include <vector>
#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"

class ComponentMesh : public Component
{
public:
	enum BlendType 
	{
		BLEND_GL_ZERO, //GL_ZERO
		BLEND_GL_ONE, //GL_ONE
		BLEND_GL_SRC_COLOR, //GL_SRC_COLOR
		BLEND_GL_ONE_MINUS_SRC_COLOR, //GL_ONE_MINUS_SRC_COLOR
		BLEND_GL_DST_COLOR, //GL_DST_COLOR
		BLEND_GL_ONE_MINUS_DST_COLOR, //GL_ONE_MINUS_DST_COLOR
		BLEND_GL_SRC_ALPHA, //GL_SRC_ALPHA
		BLEND_GL_ONE_MINUS_SRC_ALPHA, //GL_ONE_MINUS_SRC_ALPHA
		BLEND_GL_DST_ALPHA, //GL_DST_ALPHA
		BLEND_GL_ONE_MINUS_DST_ALPHA, //GL_ONE_MINUS_DST_ALPHA
		BLEND_GL_CONSTANT_COLOR, //GL_CONSTANT_COLOR
		BLEND_GL_ONE_MINUS_CONSTANT_COLOR, //GL_ONE_MINUS_CONSTANT_COLOR
		BLEND_GL_CONSTANT_ALPHA, //GL_CONSTANT_ALPHA
		BLEND_GL_ONE_MINUS_CONSTANT_ALPHA //GL_ONE_MINUS_CONSTANT_ALPHA
	};

	ALIGN_CLASS_TO_16

	ComponentMesh();
	~ComponentMesh();

	const char* name = "Unnamed";
	AABB original_bbox;
	float alpha = 0.0f;
	BlendType sfactor = BLEND_GL_SRC_ALPHA;
	BlendType dfactor = BLEND_GL_ONE_MINUS_SRC_ALPHA;

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

	void SetSfactor(int blend_type);
	void SetDfactor(int blend_type);

	std::vector<const char*> blend_types;
};

#endif