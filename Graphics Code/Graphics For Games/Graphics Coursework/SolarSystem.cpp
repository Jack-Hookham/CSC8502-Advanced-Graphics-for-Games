#include "SolarSystem.h"

SolarSystem::SolarSystem()
{	
	//Populate object information
	for (int i = 0; i < NUM_NAMES; ++i)
	{
		solarObjects[i] = new SolarObject(createSphereObj(), objectColours[i], types[i]); 
		solarObjects[i]->SetModelScale(Vector3(modelScales[i], modelScales[i], modelScales[i]));
		solarObjects[i]->SetTransform(Matrix4::Translation(Vector3(orbitRadii[i], 0.0f, 0.0f)));
	}

	//Optional, uncomment if you want a local origin marker!
	//root = createSphereObj();
	//SetMesh(root);

	sun = new SolarObject(createSphereObj(), Vector4(0.9f, 0.7f, 0.3f, 1.0f), SolarType::TYPE_SUN);
	sun->SetModelScale(Vector3(100.0f, 100.0f, 100.0f));
	//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
	sun->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f)));
	sun->SetBoundingRadius(100.0f);
	AddChild(sun);

	planet = new SolarObject(createSphereObj(), Vector4(0.2f, 0.7f, 0.9f, 1.0f), SolarType::TYPE_PLANET);
	planet->SetModelScale(Vector3(20.0f, 20.0f, 20.0f));
	//planet->SetModelScale(Vector3(2.0f, 2.0f, 2.0f));
	planet->SetTransform(Matrix4::Translation(Vector3(600.0f, 0.0f, 0.0f)));
	sun->SetBoundingRadius(20.0f);
	sun->AddChild(planet);
	

	planet2 = new SolarObject(createSphereObj(), Vector4(0.2f, 0.9f, 0.7f, 1.0f), SolarType::TYPE_PLANET);
	planet2->SetModelScale(Vector3(40.0f, 40.0f, 40.0f));
	//planet2->SetModelScale(Vector3(4.0f, 4.0f, 4.0f));
	planet2->SetTransform(Matrix4::Translation(Vector3(900.0f, 0.0f, 0.0f)));
	sun->SetBoundingRadius(40.0f);
	sun->AddChild(planet2);

	planet3 = new SolarObject(createSphereObj(), Vector4(0.2f, 0.9f, 0.7f, 1.0f), SolarType::TYPE_PLANET);
	planet3->SetModelScale(Vector3(50.0f, 50.0f, 50.0f));
	//planet3->SetModelScale(Vector3(5.0f, 5.0f, 5.0f));
	planet3->SetTransform(Matrix4::Translation(Vector3(300.0f, 0.0f, 0.0f)));
	sun->SetBoundingRadius(50.0f);
	sun->AddChild(planet3);
	
	moon = new SolarObject(createSphereObj(), Vector4(0.4f, 0.4f, 0.4f, 1.0f), SolarType::TYPE_MOON);
	moon->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
	//moon->SetModelScale(Vector3(1.0f, 1.0f, 1.0f));
	moon->SetTransform(Matrix4::Translation(Vector3(70.0f, 0.0f, 0.0f)));
	sun->SetBoundingRadius(10.0f);
	planet->AddChild(moon);

	//planet->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	//moon->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	//sun->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"sunmap.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	OBJMesh* cubeMesh = new OBJMesh();
	cubeMesh->LoadOBJMesh(MESHDIR"cube.obj");

	SolarObject* cube = new SolarObject(cubeMesh, Vector4(0.2f, 0.9f, 0.7f, 1.0f), SolarType::TYPE_PLANET);
	cube->SetModelScale(Vector3(50.0f, 50.0f, 50.0f));
	cube->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 300.0f)));
	sun->AddChild(cube);

	OBJMesh* pyramidMesh = new OBJMesh();
	pyramidMesh->LoadOBJMesh(MESHDIR"pyramid.obj");
	SolarObject* pyramid = new SolarObject(pyramidMesh, Vector4(0.2f, 0.9f, 0.7f, 1.0f), SolarType::TYPE_PLANET);
	pyramid->SetModelScale(Vector3(50.0f, 50.0f, 50.0f));
	pyramid->SetTransform(Matrix4::Translation(Vector3(0.0f, 300.0f, 0.0f)));
	sun->AddChild(pyramid);

	OBJMesh* icoMesh = new OBJMesh();
	icoMesh->LoadOBJMesh(MESHDIR"ico.obj");
	SolarObject* ico = new SolarObject(icoMesh, Vector4(0.2f, 0.9f, 0.7f, 1.0f), SolarType::TYPE_PLANET);
	ico->SetModelScale(Vector3(50.0f, 50.0f, 50.0f));
	ico->SetTransform(Matrix4::Translation(Vector3(0.0f, -300.0f, 0.0f)));
	sun->AddChild(ico);

	SolarObject* planeSouth = new SolarObject(Mesh::GenerateQuad(), Vector4(1.0f, 1.0f, 1.0f, 1.0f), SolarType::TYPE_PLANET);
	planeSouth->SetModelScale(Vector3(1000.0f, 1000.0f, 1000.0f));
	//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
	planeSouth->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 2000.0f)));
	AddChild(planeSouth);

	SolarObject* planeNorth = new SolarObject(Mesh::GenerateQuad(), Vector4(1.0f, 1.0f, 1.0f, 1.0f), SolarType::TYPE_PLANET);
	planeNorth->SetModelScale(Vector3(1000.0f, 1000.0f, 1000.0f));
	//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
	planeNorth->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, -2000.0f)));
	AddChild(planeNorth);

	SolarObject* planeEast = new SolarObject(Mesh::GenerateQuad(), Vector4(1.0f, 1.0f, 1.0f, 1.0f), SolarType::TYPE_PLANET);
	planeEast->SetModelScale(Vector3(1000.0f, 1000.0f, 1000.0f));
	//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
	planeEast->SetTransform(Matrix4::Translation(Vector3(2000.0f, 0.0f, 0.0f)) * Matrix4::Rotation(-90.0f, Vector3(0, 1, 0)));
	AddChild(planeEast);

	SolarObject* planeWest = new SolarObject(Mesh::GenerateQuad(), Vector4(1.0f, 1.0f, 1.0f, 1.0f), SolarType::TYPE_PLANET);
	planeWest->SetModelScale(Vector3(1000.0f, 1000.0f, 1000.0f));
	//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
	planeWest->SetTransform(Matrix4::Translation(Vector3(-2000.0f, 0.0f, 0.0f)) * Matrix4::Rotation(90.0f, Vector3(0, 1, 0)));
	AddChild(planeWest);

	SolarObject* planeUp = new SolarObject(Mesh::GenerateQuad(), Vector4(1.0f, 1.0f, 1.0f, 1.0f), SolarType::TYPE_PLANET);
	planeUp->SetModelScale(Vector3(1000.0f, 1000.0f, 1000.0f));
	//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
	planeUp->SetTransform(Matrix4::Translation(Vector3(0.0f, -2000.0f, 0.0f)) * Matrix4::Rotation(90.0f, Vector3(1, 0, 0)));
	AddChild(planeUp);

	SolarObject* planeDown = new SolarObject(Mesh::GenerateQuad(), Vector4(1.0f, 1.0f, 1.0f, 1.0f), SolarType::TYPE_PLANET);
	planeDown->SetModelScale(Vector3(1000.0f, 1000.0f, 1000.0f));
	//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
	planeDown->SetTransform(Matrix4::Translation(Vector3(0.0f, 2000.0f, 0.0f)) * Matrix4::Rotation(90.0f, Vector3(1, 0, 0)));
	AddChild(planeDown);
}

SolarSystem::~SolarSystem()
{
}

void SolarSystem::Update(float msec)
{
	//transform = transform * Matrix4::Rotation(msec / 10.0f, Vector3(0, 1, 0));
	float sunRotate = 1.0f;
	float earthOrbitSpeed = 0.2f;
	float planet2OrbitSpeed = 0.2f;
	float planet3OrbitSpeed = 0.2f;
	float moonOrbitSpeed = 1.3f;

	if (rotateObjects)
	{
		sun->SetTransform(sun->GetTransform() * Matrix4::Rotation(sunRotate, Vector3(0, 1, 0)));

		planet->SetTransform(planet->GetTransform() * Matrix4::Translation(Vector3(-600.0f, 0.0f, 0.0f)) *
			Matrix4::Rotation(-sunRotate + earthOrbitSpeed, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(600.0f, 0.0f, 0.0f)));

		planet2->SetTransform(planet2->GetTransform() * Matrix4::Translation(Vector3(-900.0f, 0.0f, 0.0f)) *
			Matrix4::Rotation(-sunRotate + planet2OrbitSpeed, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(900.0f, 0.0f, 0.0f)));

		planet3->SetTransform(planet3->GetTransform() * Matrix4::Translation(Vector3(-300.0f, 0.0f, 0.0f)) *
			Matrix4::Rotation(-sunRotate + planet3OrbitSpeed, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(300.0f, 0.0f, 0.0f)));

		moon->SetTransform(moon->GetTransform() * Matrix4::Translation(Vector3(-70.0f, 0.0f, 0.0f)) *
			Matrix4::Rotation(-earthOrbitSpeed + moonOrbitSpeed, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(70.0f, 0.0f, 0.0f)));
	}

	SolarObject::Update(msec);
}

Mesh* SolarSystem::createSphereObj()
{
	//Tri planar mapping?
	OBJMesh* m = new OBJMesh();
	//m->LoadOBJMesh(MESHDIR"sphere2.obj");
	//m->LoadOBJMesh(MESHDIR"mySphere.obj");
	//m->LoadOBJMesh(MESHDIR"sphereDLed.obj");
	//m->LoadOBJMesh(MESHDIR"sphere.obj");
	m->LoadOBJMesh(MESHDIR"smoothSphere.obj");
	

	return m;
}
