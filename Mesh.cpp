#include "Mesh.h"

Mesh::Mesh()
{
	type = MESH_COMPONENT;
}

Mesh::~Mesh()
{
	RELEASE_ARRAY(indices);
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(colors);
	RELEASE_ARRAY(texture_coordinates);
}