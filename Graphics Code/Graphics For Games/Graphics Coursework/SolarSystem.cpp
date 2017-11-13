#include "SolarSystem.h"

SolarSystem::SolarSystem()
{	
	//Optional, uncomment if you want a local origin marker!
	//root = createSphereObj();
	//SetMesh(root);

	sun = new SceneNode(createSphereObj(), Vector4(0.9f, 0.7f, 0.3f, 1.0f));
	sun->SetModelScale(Vector3(100.0f, 100.0f, 100.0f));
	sun->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f)));
	AddChild(sun);

	planet = new SceneNode(createSphereObj(), Vector4(0.2f, 0.7f, 0.9f, 1.0f));
	planet->SetModelScale(Vector3(20.0f, 20.0f, 20.0f));
	planet->SetTransform(Matrix4::Translation(Vector3(300.0f, 0.0f, 0.0f)));
	sun->AddChild(planet);

	planet2 = new SceneNode(createSphereObj(), Vector4(0.2f, 0.9f, 0.7f, 1.0f));
	planet2->SetModelScale(Vector3(40.0f, 40.0f, 40.0f));
	planet2->SetTransform(Matrix4::Translation(Vector3(600.0f, 0.0f, 0.0f)));
	sun->AddChild(planet2);

	Mesh* ringMesh;
	ringMesh = Mesh::GenerateQuad();
	ring = new SceneNode(ringMesh, Vector4(0.5, 0.2, 0.7, 1.0));
	ring->SetModelScale(Vector3(80.0f, 80.0f, 80.0f));
	ring->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"SaturnRing.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	ring->SetTransform(Matrix4::Translation(Vector3(600.0f, 0.0f, 0.0f)) * Matrix4::Rotation(90.0f, Vector3(1.0f, 0.0f, 0.0f)));
	//planet2->AddChild(ring);
	
	moon = new SceneNode(createSphereObj(), Vector4(0.4f, 0.4f, 0.4f, 1.0f));
	moon->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
	moon->SetTransform(Matrix4::Translation(Vector3(70.0f, 0.0f, 0.0f)));
	planet->AddChild(moon);

	//planet->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	//moon->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	//sun->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"sunmap.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
}

SolarSystem::~SolarSystem()
{
}

void SolarSystem::Update(float msec)
{
	//transform = transform * Matrix4::Rotation(msec / 10.0f, Vector3(0, 1, 0));
	float sunRotate = 1.0f;
	float earthOrbitSpeed = 0.5f;
	float planet2OrbitSpeed = 0.4f;
	float moonOrbitSpeed = 1.3f;

	//sun->SetTransform(sun->GetTransform() * Matrix4::Rotation(sunRotate, Vector3(0, 1, 0)));

	//planet->SetTransform(planet->GetTransform() * Matrix4::Translation(Vector3(-300.0f, 0.0f, 0.0f)) *
	//	Matrix4::Rotation(-sunRotate + earthOrbitSpeed, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(300.0f, 0.0f, 0.0f)));

	//planet2->SetTransform(planet2->GetTransform() * Matrix4::Translation(Vector3(-600.0f, 0.0f, 0.0f)) *
	//	Matrix4::Rotation(-sunRotate + planet2OrbitSpeed, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(600.0f, 0.0f, 0.0f)));

	//moon->SetTransform(moon->GetTransform() * Matrix4::Translation(Vector3(-70.0f, 0.0f, 0.0f)) *
	//	Matrix4::Rotation(-earthOrbitSpeed + moonOrbitSpeed, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(70.0f, 0.0f, 0.0f)));

	SceneNode::Update(msec);
}

Mesh* SolarSystem::createSphereObj()
{
	//Tri planar mapping?
	OBJMesh* m = new OBJMesh();
//	m->LoadOBJMesh(MESHDIR"testSphere.obj");
	m->LoadOBJMesh(MESHDIR"sphere.obj");
	return m;
}
