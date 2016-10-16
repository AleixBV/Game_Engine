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

bool ModuleGeometryLoader::LoadGeometryFromFile(const char* path, std::vector<Mesh*>* meshes)
{
	bool ret = true;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int x = scene->mNumMeshes, i = 0; x > 0; x--, i++)
		{
			scene->mMeshes[i];
			
			Mesh* mesh = new Mesh();
			// copy vertices
			
			mesh->num_vertices = scene->mMeshes[i]->mNumVertices;
			mesh->vertices = new float[mesh->num_vertices * 3];
			memcpy(mesh->vertices, scene->mMeshes[i]->mVertices, sizeof(float) * mesh->num_vertices * 3);
			LOG("New mesh with %d vertices", mesh->num_vertices);

			glGenBuffers(1, (GLuint*)&(mesh->id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->num_vertices, mesh->vertices, GL_STATIC_DRAW);

			// copy faces

			if (scene->mMeshes[i]->HasFaces())
			{
				mesh->num_indices = scene->mMeshes[i]->mNumFaces * 3;
				mesh->indices = new uint[mesh->num_indices]; // assume each face is a triangle
				for (uint y = 0; y < scene->mMeshes[i]->mNumFaces; y++)
				{
					if (scene->mMeshes[i]->mFaces[y].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
						memcpy(&mesh->indices[y * 3], scene->mMeshes[i]->mFaces[y].mIndices, 3 * sizeof(uint));
				}
			}

			glGenBuffers(1, (GLuint*)&(mesh->id_indices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh->num_indices, mesh->indices, GL_STATIC_DRAW);

			// copy normals

			if (scene->mMeshes[i]->HasNormals())
			{
				mesh->normals = new float[mesh->num_vertices * 3];
				memcpy(mesh->normals, scene->mMeshes[i]->mNormals, sizeof(float) * mesh->num_vertices * 3);

				glGenBuffers(1, (GLuint*)&(mesh->id_normals));
				glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->num_vertices, mesh->normals, GL_STATIC_DRAW);
			}

			// copy colors

			if (scene->mMeshes[i]->HasVertexColors(0))
			{
				mesh->colors = new float[mesh->num_vertices * 3];
				memcpy(mesh->colors, scene->mMeshes[i]->mColors, sizeof(float) * mesh->num_vertices * 3);

				glGenBuffers(1, (GLuint*)&(mesh->id_colors));
				glBindBuffer(GL_ARRAY_BUFFER, mesh->id_colors);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->num_vertices, mesh->colors, GL_STATIC_DRAW);
			}

			// copy texture coordinates

			if (scene->mMeshes[i]->HasTextureCoords(0))
			{
				mesh->texture_coordinates = new float[mesh->num_vertices * 3];
				memcpy(mesh->texture_coordinates, scene->mMeshes[i]->mTextureCoords[0], sizeof(float) * mesh->num_vertices * 3);

				glGenBuffers(1, (GLuint*)&(mesh->id_texture_coordinates));
				glBindBuffer(GL_ARRAY_BUFFER, mesh->id_texture_coordinates);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh->num_vertices, mesh->texture_coordinates, GL_STATIC_DRAW);
			}


			meshes->push_back(mesh);
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