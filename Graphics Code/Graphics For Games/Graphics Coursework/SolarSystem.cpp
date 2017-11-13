#include "SolarSystem.h"

Mesh* SolarSystem::sphere = NULL;

SolarSystem::SolarSystem()
{	
	//Optional, uncomment if you want a local origin marker!
	SetMesh(sphere);

	sun = new SceneNode(sphere, Vector4(0.9f, 0.7f, 0.3f, 1.0f));
	sun->SetModelScale(Vector3(100.0f, 100.0f, 100.0f));
	sun->SetTransform(Matrix4::Translation(Vector3(0.0f, 35.0f, 0.0f)));
	AddChild(sun);

	planet = new SceneNode(sphere, Vector4(0.2f, 0.7f, 0.9f, 1.0f));
	planet->SetModelScale(Vector3(20.0f, 20.0f, 20.0f));
	planet->SetTransform(Matrix4::Translation(Vector3(300.0f, 0.0f, 0.0f)));
	sun->AddChild(planet);

	planet2 = new SceneNode(sphere, Vector4(0.2f, 0.9f, 0.7f, 1.0f));
	planet2->SetModelScale(Vector3(40.0f, 40.0f, 40.0f));
	planet2->SetTransform(Matrix4::Translation(Vector3(600.0f, 0.0f, 0.0f)));
	sun->AddChild(planet2);
	
	moon = new SceneNode(sphere, Vector4(0.4f, 0.4f, 0.4f, 1.0f));
	moon->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
	moon->SetTransform(Matrix4::Translation(Vector3(70.0f, 0.0f, 0.0f)));
	planet->AddChild(moon);
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

	sun->SetTransform(sun->GetTransform() * Matrix4::Rotation(sunRotate, Vector3(0, 1, 0)));

	planet->SetTransform(planet->GetTransform() * Matrix4::Translation(Vector3(-300.0f, 0.0f, 0.0f)) *
		Matrix4::Rotation(-sunRotate + earthOrbitSpeed, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(300.0f, 0.0f, 0.0f)));

	planet2->SetTransform(planet2->GetTransform() * Matrix4::Translation(Vector3(-600.0f, 0.0f, 0.0f)) *
		Matrix4::Rotation(-sunRotate + planet2OrbitSpeed, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(600.0f, 0.0f, 0.0f)));

	moon->SetTransform(moon->GetTransform() * Matrix4::Translation(Vector3(-70.0f, 0.0f, 0.0f)) *
		Matrix4::Rotation(-earthOrbitSpeed + moonOrbitSpeed, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(70.0f, 0.0f, 0.0f)));

	SceneNode::Update(msec);
}

void SolarSystem::createSphereObj()
{
	OBJMesh* m = new OBJMesh();
	m->LoadOBJMesh(MESHDIR"sphereTest.obj");
	sphere = m;
}

void SolarSystem::deleteSphereObj()
{
	delete sphere;
}
