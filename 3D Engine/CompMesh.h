#ifndef COMPMESH_H
#define COMPMESH_H

#include "Globals.h"
#include <vector>
#include "glmath.h"
#include "MathGeoLib/MathGeoLib.h"
#include "Component.h"

struct Mesh
{
	uint id_vertices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_indices = 0;
	uint num_vertices = 0;
	uint* vertices = nullptr;

	uint id_normals = 0;
	uint num_normals = 0;
	float* normals = nullptr;

	uint id_uvs = 0;
	uint num_uvs = 0;
	float* uvs = nullptr;
};

class GameObject;

class CompMesh : public Component
{
public:
	CompMesh(GameObject* parent, COMP_TYPE type);
	~CompMesh();

	void Update(float dt);
	void Draw();
	void AddMesh(Mesh* mesh);
	void ApplyTexture(const char* path);

	// Read
	uint GetNumIndices()const;
	uint GetNumVertices()const;
	float GetNumNormals()const;
	float GetNumUvs()const;
	math::AABB GetAABB()const;

	// UI 
	void BlitComponentInspector();
private:
	Mesh* mesh;
};

#endif // !COMPMESH_H