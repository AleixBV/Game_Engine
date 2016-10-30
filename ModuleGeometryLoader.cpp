#include "Globals.h"
#include "Application.h"
#include "ModuleGeometryLoader.h"
#include "Mesh.h"
#include "ComponentTransform.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "Glew/include/glew.h"

#include <string>

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Glew/libx86/glew32.lib")

//Constructor
ModuleGeometryLoader::ModuleGeometryLoader(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

//Destructor
ModuleGeometryLoader::~ModuleGeometryLoader()
{

}

bool ModuleGeometryLoader::Init()
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleGeometryLoader::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();

	return true;
}

GameObject* ModuleGeometryLoader::RecursiveLoadGeometryFromFile(const aiScene* scene, const aiNode* node, GameObject* parent)
{
	aiVector3D translation;
	aiVector3D scaling;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scaling, rotation, translation);
	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

	static std::string name;
	name = (node->mName.length > 0) ? node->mName.C_Str() : "Unnamed";

	static const char* dummies[5] = {
		"$AssimpFbx$_PreRotation", "$AssimpFbx$_Rotation", "$AssimpFbx$_PostRotation",
		"$AssimpFbx$_Scaling", "$AssimpFbx$_Translation" };

	for (int i = 0; i < 5; ++i)
	{
		if (name.find(dummies[i]) != std::string::npos && node->mNumChildren == 1)
		{
			node = node->mChildren[0];

			node->mTransformation.Decompose(scaling, rotation, translation);
			// accumulate transform
			pos += float3(translation.x, translation.y, translation.z);
			scale = float3(scale.x * scaling.x, scale.y * scaling.y, scale.z * scaling.z);
			rot = rot * Quat(rotation.x, rotation.y, rotation.z, rotation.w);

			name = node->mName.C_Str();
			i = -1; // start over!
		}
	}

	GameObject* game_object = App->scene->CreateNewGameObject(parent, node->mName.C_Str());
	ComponentTransform* component_transform = new ComponentTransform(pos, scale, rot);
	game_object->components.push_back(component_transform);

	// Use scene->mNumMeshes to iterate on scene->mMeshes array
	for (int x = node->mNumMeshes, i = 0; x > 0; x--, i++)
	{
		scene->mMeshes[node->mMeshes[i]];
		Mesh* mesh = new Mesh();
		// copy vertices

		mesh->num_vertices = scene->mMeshes[node->mMeshes[i]]->mNumVertices;
		mesh->vertices = new float[mesh->num_vertices * 3];
		memcpy(mesh->vertices, scene->mMeshes[node->mMeshes[i]]->mVertices, sizeof(float) * mesh->num_vertices * 3);
		LOG("New mesh with %d vertices", mesh->num_vertices);

		glGenBuffers(1, (GLuint*)&(mesh->id_vertices));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->num_vertices, mesh->vertices, GL_STATIC_DRAW);

		// copy faces

		if (scene->mMeshes[node->mMeshes[i]]->HasFaces())
		{
			mesh->num_indices = scene->mMeshes[node->mMeshes[i]]->mNumFaces * 3;
			mesh->indices = new uint[mesh->num_indices]; // assume each face is a triangle
			for (uint y = 0; y < scene->mMeshes[node->mMeshes[i]]->mNumFaces; y++)
			{
				if (scene->mMeshes[node->mMeshes[i]]->mFaces[y].mNumIndices != 3)
				{
					LOG("WARNING, geometry face with != 3 indices!");
				}
				else
					memcpy(&mesh->indices[y * 3], scene->mMeshes[node->mMeshes[i]]->mFaces[y].mIndices, 3 * sizeof(uint));
			}
		}

		glGenBuffers(1, (GLuint*)&(mesh->id_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh->num_indices, mesh->indices, GL_STATIC_DRAW);

		// copy normals

		if (scene->mMeshes[node->mMeshes[i]]->HasNormals())
		{
			mesh->normals = new float[mesh->num_vertices * 3];
			memcpy(mesh->normals, scene->mMeshes[node->mMeshes[i]]->mNormals, sizeof(float) * mesh->num_vertices * 3);

			glGenBuffers(1, (GLuint*)&(mesh->id_normals));
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->num_vertices, mesh->normals, GL_STATIC_DRAW);
		}

		// copy colors

		if (scene->mMeshes[node->mMeshes[i]]->HasVertexColors(0))
		{
			mesh->colors = new float[mesh->num_vertices * 3];
			memcpy(mesh->colors, scene->mMeshes[node->mMeshes[i]]->mColors, sizeof(float) * mesh->num_vertices * 3);

			glGenBuffers(1, (GLuint*)&(mesh->id_colors));
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_colors);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->num_vertices, mesh->colors, GL_STATIC_DRAW);
		}

		// copy texture coordinates

		if (scene->mMeshes[node->mMeshes[i]]->HasTextureCoords(0))
		{
			mesh->texture_coordinates = new float[mesh->num_vertices * 3];
			memcpy(mesh->texture_coordinates, scene->mMeshes[node->mMeshes[i]]->mTextureCoords[0], sizeof(float) * mesh->num_vertices * 3);

			glGenBuffers(1, (GLuint*)&(mesh->id_texture_coordinates));
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_texture_coordinates);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->num_vertices, mesh->texture_coordinates, GL_STATIC_DRAW);
		}

		mesh->name = (scene->mMeshes[node->mMeshes[i]]->mName.length > 0) ? scene->mMeshes[node->mMeshes[i]]->mName.C_Str() : "Unnamed";

		game_object->components.push_back(mesh);
	}

	if (node->mNumChildren != 0)
	{
		for (int z = 0; z < node->mNumChildren; z++)
		{
			game_object->children.push_back(RecursiveLoadGeometryFromFile(scene, node->mChildren[z], game_object));
		}
	}
	return game_object;
}

bool ModuleGeometryLoader::LoadGeometryFromFile(const char* path, GameObject* root)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		root->children.push_back(RecursiveLoadGeometryFromFile(scene, scene->mRootNode));
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", path);
		ret = false;
	}
	return ret;
}