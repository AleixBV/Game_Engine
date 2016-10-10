#include "Globals.h"
#include "Application.h"
#include "ModuleGeometryLoader.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "Glew/include/glew.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Glew/libx86/glew32.lib")

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
	//RELEASE_ARRAY(indices);
	//RELEASE_ARRAY(vertices);
}

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

update_status ModuleGeometryLoader::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleGeometryLoader::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}

update_status ModuleGeometryLoader::PostUpdate(float dt)
{
	for (unsigned int x = meshes.size(), i = 0; x > 0; x--, i++)
	{
		App->renderer3D->Draw(&meshes.at(i));
	}

	return UPDATE_CONTINUE;
}

bool ModuleGeometryLoader::LoadGeometryFromFile(const char* path)
{
	bool ret = true;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int x = scene->mNumMeshes, i = 0; x > 0; x--, i++)
		{
			scene->mMeshes[i];
			
			Mesh mesh;
			// copy vertices
			
			mesh.num_vertices = scene->mMeshes[i]->mNumVertices;
			mesh.vertices = new float[mesh.num_vertices * 3];
			memcpy(mesh.vertices, scene->mMeshes[i]->mVertices, sizeof(float) * mesh.num_vertices * 3);
			LOG("New mesh with %d vertices", mesh.num_vertices);

			glGenBuffers(1, (GLuint*)&(mesh.id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.num_vertices, mesh.vertices, GL_STATIC_DRAW);

			// copy faces

			if (scene->mMeshes[i]->HasFaces())
			{
				mesh.num_indices = scene->mMeshes[i]->mNumFaces * 3;
				mesh.indices = new uint[mesh.num_indices]; // assume each face is a triangle
				for (uint y = 0; y < scene->mMeshes[i]->mNumFaces; y++)
				{
					if (scene->mMeshes[i]->mFaces[y].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
						memcpy(&mesh.indices[y * 3], scene->mMeshes[i]->mFaces[y].mIndices, 3 * sizeof(uint));
				}
			}

			glGenBuffers(1, (GLuint*)&(mesh.id_indices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);


			meshes.push_back(mesh);
		}

		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", path);
		ret = false;
	}

	return ret;
}