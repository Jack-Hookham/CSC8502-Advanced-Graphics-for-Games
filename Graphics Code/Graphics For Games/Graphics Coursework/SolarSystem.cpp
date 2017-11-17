#include "SolarSystem.h"

SolarSystem::SolarSystem()
{
	debugShadows = false;
	shadowShapes = false;

	//Populate object information
	for (int i = 0; i < NUM_NAMES; ++i)
	{
		RenderObjects[i] = new RenderObject(createSphereObj(), objectColours[i], types[i]); 
		RenderObjects[i]->SetModelScale(Vector3(modelScales[i], modelScales[i], modelScales[i]));
		RenderObjects[i]->SetTransform(Matrix4::Translation(Vector3(orbitRadii[i], 0.0f, 0.0f)));
	}

	//Optional, uncomment if you want a local origin marker!
	//root = createSphereObj();
	//SetMesh(root);


	sun = new RenderObject(createSphereObj(), Vector4(0.9f, 0.7f, 0.3f, 1.0f), RenderType::TYPE_SUN);
	sun->SetModelScale(Vector3(100.0f * scale, 100.0f * scale, 100.0f * scale));
	//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
	sun->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f)));
	sun->SetBoundingRadius(100.0f * scale);
	AddChild(sun);

	planet = new RenderObject(createSphereObj(), Vector4(0.2f, 0.7f, 0.9f, 1.0f), RenderType::TYPE_PLANET);
	planet->SetModelScale(Vector3(20.0f * scale, 20.0f * scale, 20.0f * scale));
	//planet->SetModelScale(Vector3(2.0f, 2.0f, 2.0f));
	planet->SetTransform(Matrix4::Translation(Vector3(600.0f * scale, 0.0f, 0.0f)));
	sun->SetBoundingRadius(20.0f * scale);
	sun->AddChild(planet);
	
	planet2 = new RenderObject(createSphereObj(), Vector4(0.2f, 0.9f, 0.7f, 1.0f), RenderType::TYPE_PLANET);
	planet2->SetModelScale(Vector3(40.0f * scale, 40.0f * scale, 40.0f * scale));
	//planet2->SetModelScale(Vector3(4.0f, 4.0f, 4.0f));
	planet2->SetTransform(Matrix4::Translation(Vector3(900.0f * scale, 0.0f, 0.0f)));
	sun->SetBoundingRadius(40.0f * scale);
	sun->AddChild(planet2);

	planet3 = new RenderObject(createSphereObj(), Vector4(0.2f, 0.9f, 0.7f, 1.0f), RenderType::TYPE_PLANET);
	planet3->SetModelScale(Vector3(50.0f * scale, 50.0f * scale, 50.0f * scale));
	//planet3->SetModelScale(Vector3(5.0f, 5.0f, 5.0f));
	planet3->SetTransform(Matrix4::Translation(Vector3(300.0f * scale, 0.0f, 0.0f)));
	sun->SetBoundingRadius(50.0f * scale);
	sun->AddChild(planet3);
	
	moon = new RenderObject(createSphereObj(), Vector4(0.4f, 0.4f, 0.4f, 1.0f), RenderType::TYPE_MOON);
	moon->SetModelScale(Vector3(10.0f * scale, 10.0f * scale, 10.0f * scale));
	//moon->SetModelScale(Vector3(1.0f, 1.0f, 1.0f));
	moon->SetTransform(Matrix4::Rotation(180.0f, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(70.0f * scale, 0.0f, 0.0f)));
	sun->SetBoundingRadius(10.0f * scale);
	planet->AddChild(moon);

	//planet->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	//moon->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	//sun->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"sunmap.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if (debugShadows)
	{
		if (shadowShapes)
		{
			OBJMesh* cubeMesh = new OBJMesh();
			cubeMesh->LoadOBJMesh(MESHDIR"cube.obj");

			RenderObject* cube = new RenderObject(cubeMesh, Vector4(0.2f, 0.9f, 0.7f, 1.0f), RenderType::TYPE_PLANET);
			cube->SetModelScale(Vector3(50.0f, 50.0f, 50.0f));
			cube->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 300.0f)));
			sun->AddChild(cube);

			OBJMesh* pyramidMesh = new OBJMesh();
			pyramidMesh->LoadOBJMesh(MESHDIR"pyramid.obj");
			RenderObject* pyramid = new RenderObject(pyramidMesh, Vector4(0.2f, 0.9f, 0.7f, 1.0f), RenderType::TYPE_PLANET);
			pyramid->SetModelScale(Vector3(50.0f, 50.0f, 50.0f));
			pyramid->SetTransform(Matrix4::Translation(Vector3(0.0f, 300.0f, 0.0f)));
			sun->AddChild(pyramid);

			OBJMesh* icoMesh = new OBJMesh();
			icoMesh->LoadOBJMesh(MESHDIR"ico.obj");
			RenderObject* ico = new RenderObject(icoMesh, Vector4(0.2f, 0.9f, 0.7f, 1.0f), RenderType::TYPE_PLANET);
			ico->SetModelScale(Vector3(50.0f, 50.0f, 50.0f));
			ico->SetTransform(Matrix4::Translation(Vector3(0.0f, -300.0f, 0.0f)));
			sun->AddChild(ico);
		}

		RenderObject* planeSouth = new RenderObject(Mesh::GenerateQuad(), Vector4(0.0f, 0.0f, 1.0f, 1.0f), RenderType::TYPE_PLANET);
		planeSouth->SetModelScale(Vector3(2000.0f, 2000.0f, 1.0f));
		//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
		planeSouth->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 2000.0f)));
		AddChild(planeSouth);

		RenderObject* planeNorth = new RenderObject(Mesh::GenerateQuad(), Vector4(0.0f, 0.0f, 1.0f, 1.0f), RenderType::TYPE_PLANET);
		planeNorth->SetModelScale(Vector3(2000.0f, 2000.0f, 1.0f));
		//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
		planeNorth->SetTransform(Matrix4::Rotation(180.0f, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(0.0f, 0.0f, 2000.0f)) /* Matrix4::Rotation(180.0f, Vector3(0, 1, 0))*/);
		AddChild(planeNorth);

		RenderObject* planeEast = new RenderObject(Mesh::GenerateQuad(), Vector4(0.0f, 0.0f, 1.0f, 1.0f), RenderType::TYPE_PLANET);
		planeEast->SetModelScale(Vector3(2000.0f, 2000.0f, 1.0f));
		//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
		planeEast->SetTransform(Matrix4::Translation(Vector3(2000.0f, 0.0f, 0.0f)) * Matrix4::Rotation(90.0f, Vector3(0, 1, 0)));
		AddChild(planeEast);

		RenderObject* planeWest = new RenderObject(Mesh::GenerateQuad(), Vector4(0.0f, 0.0f, 1.0f, 1.0f), RenderType::TYPE_PLANET);
		planeWest->SetModelScale(Vector3(2000.0f, 2000.0f, 1.0f));
		//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
		planeWest->SetTransform(Matrix4::Rotation(180.0f, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(2000.0f, 0.0f, 0.0f)) * Matrix4::Rotation(90.0f, Vector3(0, 1, 0)));
		AddChild(planeWest);

		RenderObject* planeUp = new RenderObject(Mesh::GenerateQuad(), Vector4(0.0f, 0.0f, 1.0f, 1.0f), RenderType::TYPE_PLANET);
		planeUp->SetModelScale(Vector3(500.0f, 500.0f, 1.0f));
		//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
		planeUp->SetTransform(Matrix4::Translation(Vector3(0.0f, 2000.0f, 0.0f)) * Matrix4::Rotation(90.0f, Vector3(1, 0, 0)));
		AddChild(planeUp);

		RenderObject* planeDown = new RenderObject(Mesh::GenerateQuad(), Vector4(0.0f, 0.0f, 1.0f, 1.0f), RenderType::TYPE_PLANET);
		planeDown->SetModelScale(Vector3(500.0f, 500.0f, 1.0f));
		//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
		planeDown->SetTransform(Matrix4::Rotation(180.0f, Vector3(0, 0, 1)) * Matrix4::Translation(Vector3(0.0f, 2000.0f, 0.0f)) * Matrix4::Rotation(90.0f, Vector3(1, 0, 0)));
		AddChild(planeDown);
	}
}

SolarSystem::~SolarSystem()
{
}

void SolarSystem::Update(float msec)
{
	//transform = transform * Matrix4::Rotation(msec / 10.0f, Vector3(0, 1, 0));
	float sunRotate = 0.7f;
	float earthOrbitSpeed = 0.3f;
	float planet2OrbitSpeed = 0.4f;
	float planet3OrbitSpeed = 0.2f;
	float moonOrbitSpeed = 1.3f;

	if (rotateObjects)
	{
		sun->SetTransform(sun->GetTransform() * Matrix4::Rotation(sunRotate, Vector3(0, 1, 0)));
		//sun->SetTransform(sun->GetTransform() * Matrix4::Translation(Vector3(10.1f, 0, 0)));

		planet->SetTransform(planet->GetTransform() * Matrix4::Translation(Vector3(-600.0f * scale, 0.0f, 0.0f)) *
			Matrix4::Rotation(-sunRotate + earthOrbitSpeed, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(600.0f * scale, 0.0f, 0.0f)));

		planet2->SetTransform(planet2->GetTransform() * Matrix4::Translation(Vector3(-900.0f * scale, 0.0f, 0.0f)) *
			Matrix4::Rotation(-sunRotate + planet2OrbitSpeed, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(900.0f * scale, 0.0f, 0.0f)));

		planet3->SetTransform(planet3->GetTransform() * Matrix4::Translation(Vector3(-300.0f * scale, 0.0f, 0.0f)) *
			Matrix4::Rotation(-sunRotate + planet3OrbitSpeed, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(300.0f * scale, 0.0f, 0.0f)));

		//moon->SetTransform(moon->GetTransform() * Matrix4::Translation(Vector3(-70.0f, 0.0f, 0.0f)) *
		//	Matrix4::Rotation(-earthOrbitSpeed + moonOrbitSpeed, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(70.0f, 0.0f, 0.0f)));
	}

	RenderObject::Update(msec);
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
