#include "ComponentMesh.h"

ComponentMesh::ComponentMesh()
{
	type = MESH_COMPONENT;

	blend_types.push_back("GL_ZERO");
	blend_types.push_back("GL_ONE");
	blend_types.push_back("GL_SRC_COLOR");
	blend_types.push_back("GL_ONE_MINUS_SRC_COLOR");
	blend_types.push_back("GL_DST_COLOR");
	blend_types.push_back("GL_ONE_MINUS_DST_COLOR");
	blend_types.push_back("GL_SRC_ALPHA");
	blend_types.push_back("GL_ONE_MINUS_SRC_ALPHA");
	blend_types.push_back("GL_DST_ALPHA");
	blend_types.push_back("GL_ONE_MINUS_DST_ALPHA");
	blend_types.push_back("GL_CONSTANT_COLOR");
	blend_types.push_back("GL_ONE_MINUS_CONSTANT_COLOR");
	blend_types.push_back("GL_CONSTANT_ALPHA");
	blend_types.push_back("GL_ONE_MINUS_CONSTANT_ALPHA");
}

ComponentMesh::~ComponentMesh()
{
	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(colors);
	RELEASE_ARRAY(texture_coordinates);
}

void ComponentMesh::SetSfactor(int blend_type)
{
	switch (blend_type)
	{
	case 0:
		sfactor = BLEND_GL_ZERO;
		break;
	case 1:
		sfactor = BLEND_GL_ONE;
		break;
	case 2:
		sfactor = BLEND_GL_SRC_COLOR;
		break;
	case 3:
		sfactor = BLEND_GL_ONE_MINUS_SRC_COLOR;
		break;
	case 4:
		sfactor = BLEND_GL_DST_COLOR;
		break;
	case 5:
		sfactor = BLEND_GL_ONE_MINUS_DST_COLOR;
		break;
	case 6:
		sfactor = BLEND_GL_SRC_ALPHA;
		break;
	case 7:
		sfactor = BLEND_GL_ONE_MINUS_SRC_ALPHA;
		break;
	case 8:
		sfactor = BLEND_GL_DST_ALPHA;
		break;
	case 9:
		sfactor = BLEND_GL_ONE_MINUS_DST_ALPHA;
		break;
	case 10:
		sfactor = BLEND_GL_CONSTANT_COLOR;
		break;
	case 11:
		sfactor = BLEND_GL_ONE_MINUS_CONSTANT_COLOR;
		break;
	case 12:
		sfactor = BLEND_GL_CONSTANT_ALPHA;
		break;
	case 13:
		sfactor = BLEND_GL_ONE_MINUS_CONSTANT_ALPHA;
		break;
	default:
		sfactor = BLEND_GL_SRC_ALPHA;
		break;
	}
}

void ComponentMesh::SetDfactor(int blend_type)
{
	switch (blend_type)
	{
	case 0:
		dfactor = BLEND_GL_ZERO;
		break;
	case 1:
		dfactor = BLEND_GL_ONE;
		break;
	case 2:
		dfactor = BLEND_GL_SRC_COLOR;
		break;
	case 3:
		dfactor = BLEND_GL_ONE_MINUS_SRC_COLOR;
		break;
	case 4:
		dfactor = BLEND_GL_DST_COLOR;
		break;
	case 5:
		dfactor = BLEND_GL_ONE_MINUS_DST_COLOR;
		break;
	case 6:
		dfactor = BLEND_GL_SRC_ALPHA;
		break;
	case 7:
		dfactor = BLEND_GL_ONE_MINUS_SRC_ALPHA;
		break;
	case 8:
		dfactor = BLEND_GL_DST_ALPHA;
		break;
	case 9:
		dfactor = BLEND_GL_ONE_MINUS_DST_ALPHA;
		break;
	case 10:
		dfactor = BLEND_GL_CONSTANT_COLOR;
		break;
	case 11:
		dfactor = BLEND_GL_ONE_MINUS_CONSTANT_COLOR;
		break;
	case 12:
		dfactor = BLEND_GL_CONSTANT_ALPHA;
		break;
	case 13:
		dfactor = BLEND_GL_ONE_MINUS_CONSTANT_ALPHA;
		break;
	default:
		dfactor = BLEND_GL_SRC_ALPHA;
		break;
	}
}
