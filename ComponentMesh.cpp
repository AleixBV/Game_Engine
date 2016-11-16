#include "ComponentMesh.h"

ComponentMesh::ComponentMesh()
{
	type = MESH_COMPONENT;
}

ComponentMesh::~ComponentMesh()
{
	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(colors);
	RELEASE_ARRAY(texture_coordinates);
}