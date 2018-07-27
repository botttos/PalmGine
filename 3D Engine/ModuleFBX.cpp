#include "Application.h"
#include "ModuleFBX.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Devil/include/il.h"
#include "Devil/include/ilut.h"
#include "MathGeoLib\Geometry\AABB.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment ( lib, "Devil/libx86/ILU.lib" )
#pragma comment ( lib, "Devil/libx86/ILUT.lib" )

using namespace std;

ModuleFBX::ModuleFBX(bool start_enabled) : Module(start_enabled)
{}

ModuleFBX::~ModuleFBX()
{}

bool ModuleFBX::Start()
{
	bool ret = true;

	ilInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	struct aiLogStream stream = aiLogStream();
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

update_status ModuleFBX::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleFBX::ClearMeshes()
{
	for (int i = meshes.size() - 1; meshes.size() != 0; i--)
	{
		delete[] meshes[i].indices;
		delete[] meshes[i].vertices;
		meshes.pop_back();
	}

	delete data.indices;
	delete data.vertices;
}

bool ModuleFBX::CleanUp()
{
	aiDetachAllLogStreams();
	ClearMeshes();

	return true;
}

bool ModuleFBX::LoadFBX(const char* path)
{
	LOG("Loading the FBX...");
	file_name.clear();
	this->path = path;
	std::string name(path);
	//Delete rest of the path to substract the name of the file
	this->file_name = name.substr(name.find_last_of('\\') + 1); 
	bool ret = true;
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		aiNode* rootNode = scene->mRootNode;

		for (int i = 0; i < rootNode->mNumChildren; ++i)
		{
			LoadModel(scene, rootNode->mChildren[i], path);
		}

		aiReleaseImport(scene);
		LOG("FBX loaded correctly --------------");
		return ret;
	}
	else
		LOG("ERROR, COULDN'T LOAD FBX ---------------");
	return false;
}

void ModuleFBX::LoadModel(const aiScene* scene, aiNode* node, const char* path)
{
	if (node->mNumMeshes <= 0)
	{
		LOG("Unable to load the mesh with path: %s. The number of meshes is below or equal to 0.", path);
	}
	else
	{
		LOG("Loading mesh from path %s", path);
		for (int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* new_mesh = scene->mMeshes[node->mMeshes[i]];
			mesh = ModelConfig();
			mesh.num_vertices = new_mesh->mNumVertices;
			mesh.vertices = new uint[mesh.num_vertices * 3];
			memcpy(mesh.vertices, new_mesh->mVertices, sizeof(float)*mesh.num_vertices * 3);

			glGenBuffers(1, (GLuint*)&(mesh.id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.num_vertices, mesh.vertices, GL_STATIC_DRAW);

			if (new_mesh->HasFaces())
			{
				mesh.num_indices = new_mesh->mNumFaces * 3;
				mesh.indices = new uint[mesh.num_indices];
				for (uint j = 0; j < new_mesh->mNumFaces; j++)
				{
					if (new_mesh->mFaces[j].mNumIndices == 3)
					{
						memcpy(&mesh.indices[j * 3], new_mesh->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}

				glGenBuffers(1, (GLuint*)&(mesh.id_indices));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);
			}
			else
			{
				LOG("Mesh with %i faces can not be loaded.", new_mesh->mNumFaces);
			}

			aiMaterial* material = scene->mMaterials[new_mesh->mMaterialIndex];
  
			meshes.push_back(mesh);
			LOG("Loaded mesh with %i vertices.", mesh.num_vertices);
			LOG("Loaded mesh with %i indices.", mesh.num_indices);
			LOG("Loaded mesh with %i triangles.", mesh.num_vertices / 3);
		}
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		LoadModel(scene, node->mChildren[i], path);
	}

	mesh.position = (0.f, 0.f, 0.f);
	mesh.rotation = (0.f, 0.f, 0.f);
	mesh.scale = (1.f, 1.f, 1.f);

	LOG("Mesh position: (%f, %f, %f)", mesh.position.x, mesh.position.y, mesh.position.z);
	LOG("Mesh rotation: (%f, %f, %f)", mesh.rotation.x, mesh.rotation.y, mesh.rotation.z);
	LOG("Mesh scale: (%f, %f, %f)", mesh.scale.x, mesh.scale.y, mesh.scale.z);
}

void const ModuleFBX::CentrateObjectView()
{

}

uint const ModuleFBX::GetIndices()
{
	return(mesh.num_indices);
}

uint const ModuleFBX::GetVertices()
{
	return(mesh.num_vertices);
}

vec3 const ModuleFBX::GetPosition()
{
	return(mesh.position);
}

vec3 const ModuleFBX::GetRotation()
{
	return(mesh.rotation);
}

vec3 const ModuleFBX::GetScale()
{
	return(mesh.scale);
}