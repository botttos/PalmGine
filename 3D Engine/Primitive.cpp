
#include "Globals.h"
#include "Application.h"
#include "Primitive.h"
#include "SDL\include\SDL_opengl.h"
#include <gl\GL.h>
#include <gl\GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

// ------------------------------------------------------------
Primitive::Primitive() : transform(IdentityMatrix), color(White), wire(false), axis(false), type(PrimitiveTypes::Primitive_Point)
{}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Primitive::Render() const
{
	glPushMatrix();
	glMultMatrixf(transform.M);

	if(axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);
	}

	glColor3f(color.r, color.g, color.b);

	if(wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	InnerRender();

	glPopMatrix();
}

// ------------------------------------------------------------
void Primitive::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

// ------------------------------------------------------------
void Primitive::SetPos(float x, float y, float z)
{
	transform.translate(x, y, z);
}

// ------------------------------------------------------------
void Primitive::SetRotation(float angle, const vec3 &u)
{
	transform.rotate(angle, u);
}

// ------------------------------------------------------------
void Primitive::Scale(float x, float y, float z)
{
	transform.scale(x, y, z);
}

// CUBE ============================================
Cube::Cube() : Primitive(), size(1.0f, 1.0f, 1.0f)
{
	type = PrimitiveTypes::Primitive_Cube;
	CreateBuffers();
}

Cube::Cube(float sizeX, float sizeY, float sizeZ) : Primitive(), size(sizeX, sizeY, sizeZ)
{
	type = PrimitiveTypes::Primitive_Cube;
	CreateBuffers();
}

void Cube::CreateBuffers() 
{
	sx = size.x /** 0.5f*/;
	sy = size.y /** 0.5f*/;
	sz = size.z /** 0.5f*/;

	vertices = new float[72] { sx, sy, sz,   -sx, sy, sz,    -sx, -sy, sz,    sx, -sy, sz,   // v0,v1,v2,v3 (front)
					 sx, sy, sz,    sx, -sy, sz,    sx, -sy, -sz,    sx, sy, -sz,   // v0,v3,v4,v5 (right)
					 sx, sy, sz,     sx, sy, -sz,    -sx, sy, -sz,    -sx, sy, sz,   // v0,v5,v6,v1 (top)
					 -sx, sy, sz,   -sx, sy, -sz,   -sx, -sy, -sz,   -sx, -sy, sz,   // v1,v6,v7,v2 (left)
					 -sx, -sy, -sz,  sx, -sy, -sz,   sx, -sy, sz,   -sx, -sy, sz,   // v7,v4,v3,v2 (bottom)
					  sx, -sy, -sz,  -sx, -sy, -sz,  -sx, sy, -sz,  sx, sy, -sz }; // v4,v7,v6,v5 (back)

	normals = new float[72] { 0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,   // v0,v1,v2,v3 (front)
				  1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,   // v0,v3,v4,v5 (right)
		   		  0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,   // v0,v5,v6,v1 (top)
				  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,   // v1,v6,v7,v2 (left)
				  0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,   // v7,v4,v3,v2 (bottom)
				  0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1 }; // v4,v7,v6,v5 (back)

	indices = new uint[36] { 0, 1, 2,   2, 3, 0,      // front
					4, 5, 6,   6, 7, 4,      // right
					8, 9,10,  10,11, 8,      // top
					12,13,14,  14,15,12,      // left
					16,17,18,  18,19,16,      // bottom
					20,21,22,  22,23,20 };    // back
	
	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*72, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(my_normals));
	glBindBuffer(GL_ARRAY_BUFFER, my_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 72, normals, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*36, indices, GL_STATIC_DRAW);

}

void Cube::InnerRender() const
{	
	/*float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;*/

	//glBegin(GL_QUADS);

	//glNormal3f(0.0f, 0.0f, 1.0f);
	//glVertex3f(-sx, -sy, sz);
	//glVertex3f( sx, -sy, sz);
	//glVertex3f( sx,  sy, sz);
	//glVertex3f(-sx,  sy, sz);

	//glNormal3f(0.0f, 0.0f, -1.0f);
	//glVertex3f( sx, -sy, -sz);
	//glVertex3f(-sx, -sy, -sz);
	//glVertex3f(-sx,  sy, -sz);
	//glVertex3f( sx,  sy, -sz);

	//glNormal3f(1.0f, 0.0f, 0.0f);
	//glVertex3f(sx, -sy,  sz);
	//glVertex3f(sx, -sy, -sz);
	//glVertex3f(sx,  sy, -sz);
	//glVertex3f(sx,  sy,  sz);

	//glNormal3f(-1.0f, 0.0f, 0.0f);
	//glVertex3f(-sx, -sy, -sz);
	//glVertex3f(-sx, -sy,  sz);
	//glVertex3f(-sx,  sy,  sz);
	//glVertex3f(-sx,  sy, -sz);

	//glNormal3f(0.0f, 1.0f, 0.0f);
	//glVertex3f(-sx, sy,  sz);
	//glVertex3f( sx, sy,  sz);
	//glVertex3f( sx, sy, -sz);
	//glVertex3f(-sx, sy, -sz);

	//glNormal3f(0.0f, -1.0f, 0.0f);
	//glVertex3f(-sx, -sy, -sz);
	//glVertex3f( sx, -sy, -sz);
	//glVertex3f( sx, -sy,  sz);
	//glVertex3f(-sx, -sy,  sz);

	//glEnd();

	glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, my_normals);
	glNormalPointer(GL_FLOAT, 0, normals);
	//glColorPointer(3, GL_FLOAT, 0, colors2);

	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, vertices);

	//glPushMatrix();
	//glTranslatef(-2, -2, 0);                // move to bottom-left corner

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	//glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
	//glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

// SPHERE ============================================
//Sphere::Sphere() : Primitive(), radius(1.0f)
//{
//	type = PrimitiveTypes::Primitive_Sphere;
//}
//
//Sphere::Sphere(float radius) : Primitive(), radius(radius)
//{
//	type = PrimitiveTypes::Primitive_Sphere;
//}
//
//void Sphere::InnerRender() const
//{
//	glutSolidSphere(radius, 25, 25);
//}
//
//
//// CYLINDER ============================================
//Cylinder::Cylinder() : Primitive(), radius(1.0f), height(1.0f)
//{
//	type = PrimitiveTypes::Primitive_Cylinder;
//}
//
//Cylinder::Cylinder(float radius, float height) : Primitive(), radius(radius), height(height)
//{
//	type = PrimitiveTypes::Primitive_Cylinder;
//}
//
//void Cylinder::InnerRender() const
//{
//	int n = 30;
//
//	// Cylinder Bottom
//	glBegin(GL_POLYGON);
//	
//	for(int i = 360; i >= 0; i -= (360 / n))
//	{
//		float a = i * M_PI / 180; // degrees to radians
//		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a));
//	}
//	glEnd();
//
//	// Cylinder Top
//	glBegin(GL_POLYGON);
//	glNormal3f(0.0f, 0.0f, 1.0f);
//	for(int i = 0; i <= 360; i += (360 / n))
//	{
//		float a = i * M_PI / 180; // degrees to radians
//		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
//	}
//	glEnd();
//
//	// Cylinder "Cover"
//	glBegin(GL_QUAD_STRIP);
//	for(int i = 0; i < 480; i += (360 / n))
//	{
//		float a = i * M_PI / 180; // degrees to radians
//
//		glVertex3f(height*0.5f,  radius * cos(a), radius * sin(a) );
//		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a) );
//	}
//	glEnd();
//}
//
//// LINE ==================================================
//Line::Line() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
//{
//	type = PrimitiveTypes::Primitive_Line;
//}
//
//Line::Line(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
//{
//	type = PrimitiveTypes::Primitive_Line;
//}
//
//void Line::InnerRender() const
//{
//	glLineWidth(2.0f);
//
//	glBegin(GL_LINES);
//
//	glVertex3f(origin.x, origin.y, origin.z);
//	glVertex3f(destination.x, destination.y, destination.z);
//
//	glEnd();
//
//	glLineWidth(1.0f);
//}

// PLANE ==================================================
PlaneGrid::PlaneGrid() : Primitive(), normal(0, 1, 0), constant(1)
{
	type = PrimitiveTypes::Primitive_Plane;
}

PlaneGrid::PlaneGrid(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
{
	type = PrimitiveTypes::Primitive_Plane;
}

void PlaneGrid::InnerRender() const
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	float d = 200.0f;

	for(float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}