#include "SolarSystem.h"

SolarSystem::SolarSystem()
{
	initPlanets();

	Mesh* sphereMesh = createSphereObj();

	debugShadows = false;
	shadowShapes = false;

	//Populate object information
	sun = new RenderObject(new Mesh(*sphereMesh), Vector4(0.9f, 0.7f, 0.3f, 1.0f), RenderType::TYPE_SUN);
	sun->SetModelScale(Vector3(300.0f * scale));
	sun->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f)));
	sun->SetBoundingRadius(100.0f * scale);
	sun->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"TileFire.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if (!sun->GetMesh()->GetTexture())
	{
		return;
	}
	SetTextureRepeating(sun->GetMesh()->GetTexture(), true);

	AddChild(sun);

	for (int i = 0; i < PlanetNames::NUM_PLANETS; ++i)
	{
		RenderObject* o = new RenderObject(new Mesh(*sphereMesh), planets[i].colour, RenderType::TYPE_PLANET);
		o->SetModelScale(Vector3(planets[i].bodyRadius));
		o->SetTransform(Matrix4::Translation(Vector3(planets[i].orbitRadius, 0.0f, 0.0f)));
		o->GetMesh()->SetTexture(SOIL_load_OGL_texture(planets[i].texture, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

		if (!o->GetMesh()->GetTexture())
		{
			return;
		}

		SetTextureRepeating(o->GetMesh()->GetTexture(), true);

		sun->AddChild(o);
	}

	//planet = new RenderObject(new Mesh(*sphereMesh), Vector4(0.2f, 0.7f, 0.9f, 1.0f), RenderType::TYPE_PLANET);
	//planet->SetModelScale(Vector3(20.0f * scale, 20.0f * scale, 20.0f * scale));
	////planet->SetModelScale(Vector3(2.0f, 2.0f, 2.0f));
	//planet->SetTransform(Matrix4::Translation(Vector3(300.0f * scale, 0.0f, 0.0f)));
	//sun->SetBoundingRadius(20.0f * scale);
	//sun->AddChild(planet);

	//planet2 = new RenderObject(new Mesh(*sphereMesh), Vector4(0.2f, 0.9f, 0.7f, 1.0f), RenderType::TYPE_PLANET);
	//planet2->SetModelScale(Vector3(70.0f * scale, 70.0f * scale, 70.0f * scale));
	////planet2->SetModelScale(Vector3(4.0f, 4.0f, 4.0f));
	//planet2->SetTransform(Matrix4::Translation(Vector3(900.0f * scale, 0.0f, 0.0f)));
	//sun->SetBoundingRadius(40.0f * scale);
	//sun->AddChild(planet2);

	//planet3 = new RenderObject(new Mesh(*sphereMesh), Vector4(0.2f, 0.9f, 0.7f, 1.0f), RenderType::TYPE_PLANET);
	//planet3->SetModelScale(Vector3(50.0f * scale, 50.0f * scale, 50.0f * scale));
	////planet3->SetModelScale(Vector3(5.0f, 5.0f, 5.0f));
	//planet3->SetTransform(Matrix4::Translation(Vector3(600.0f * scale, 0.0f, 0.0f)));
	//sun->SetBoundingRadius(50.0f * scale);
	//sun->AddChild(planet3);
	//
	//moon = new RenderObject(new Mesh(*sphereMesh), Vector4(0.4f, 0.4f, 0.4f, 1.0f), RenderType::TYPE_MOON);
	//moon->SetModelScale(Vector3(10.0f * scale, 10.0f * scale, 10.0f * scale));
	////moon->SetModelScale(Vector3(1.0f, 1.0f, 1.0f));
	//moon->SetTransform(Matrix4::Rotation(180.0f, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(70.0f * scale, 0.0f, 0.0f)));
	//sun->SetBoundingRadius(10.0f * scale);
	//planet->AddChild(moon);

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
			cube->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 700.0f)));
			sun->AddChild(cube);

			OBJMesh* pyramidMesh = new OBJMesh();
			pyramidMesh->LoadOBJMesh(MESHDIR"pyramid.obj");
			RenderObject* pyramid = new RenderObject(pyramidMesh, Vector4(0.2f, 0.9f, 0.1f, 1.0f), RenderType::TYPE_PLANET);
			pyramid->SetModelScale(Vector3(50.0f, 50.0f, 50.0f));
			pyramid->SetTransform(Matrix4::Translation(Vector3(0.0f, 700.0f, 0.0f)));
			sun->AddChild(pyramid);

			OBJMesh* icoMesh = new OBJMesh();
			icoMesh->LoadOBJMesh(MESHDIR"ico.obj");
			RenderObject* ico = new RenderObject(icoMesh, Vector4(0.9f, 0.9f, 0.7f, 1.0f), RenderType::TYPE_PLANET);
			ico->SetModelScale(Vector3(50.0f, 50.0f, 50.0f));
			ico->SetTransform(Matrix4::Rotation(180.0f, Vector3(1, 0, 0)) * Matrix4::Translation(Vector3(0.0f, 700.0f, 0.0f)));
			sun->AddChild(ico);
		}

		RenderObject* planeSouth = new RenderObject(Mesh::GenerateQuad(), Vector4(0.0f, 0.0f, 1.0f, 1.0f), RenderType::TYPE_PLANET);
		planeSouth->SetModelScale(Vector3(1950.0f, 1950.0f, 1.0f));
		//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
		planeSouth->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 2000.0f)));
		AddChild(planeSouth);

		RenderObject* planeNorth = new RenderObject(Mesh::GenerateQuad(), Vector4(0.0f, 0.0f, 1.0f, 1.0f), RenderType::TYPE_PLANET);
		planeNorth->SetModelScale(Vector3(1950.0f, 1950.0f, 1.0f));
		//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
		planeNorth->SetTransform(Matrix4::Rotation(180.0f, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(0.0f, 0.0f, 2000.0f)) /* Matrix4::Rotation(180.0f, Vector3(0, 1, 0))*/);
		AddChild(planeNorth);

		RenderObject* planeEast = new RenderObject(Mesh::GenerateQuad(), Vector4(0.0f, 0.0f, 1.0f, 1.0f), RenderType::TYPE_PLANET);
		planeEast->SetModelScale(Vector3(1950.0f, 1950.0f, 1.0f));
		//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
		planeEast->SetTransform(Matrix4::Translation(Vector3(2000.0f, 0.0f, 0.0f)) * Matrix4::Rotation(90.0f, Vector3(0, 1, 0)));
		AddChild(planeEast);

		RenderObject* planeWest = new RenderObject(Mesh::GenerateQuad(), Vector4(0.0f, 0.0f, 1.0f, 1.0f), RenderType::TYPE_PLANET);
		planeWest->SetModelScale(Vector3(1950.0f, 1950.0f, 1.0f));
		//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
		planeWest->SetTransform(Matrix4::Rotation(180.0f, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(2000.0f, 0.0f, 0.0f)) * Matrix4::Rotation(90.0f, Vector3(0, 1, 0)));
		AddChild(planeWest);

		RenderObject* planeUp = new RenderObject(Mesh::GenerateQuad(), Vector4(0.0f, 0.0f, 1.0f, 1.0f), RenderType::TYPE_PLANET);
		planeUp->SetModelScale(Vector3(1950.0f, 1950.0f, 1.0f));
		//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
		planeUp->SetTransform(Matrix4::Translation(Vector3(0.0f, 2000.0f, 0.0f)) * Matrix4::Rotation(-90.0f, Vector3(1, 0, 0)));
		AddChild(planeUp);

		RenderObject* planeDown = new RenderObject(Mesh::GenerateQuad(), Vector4(0.0f, 0.0f, 1.0f, 1.0f), RenderType::TYPE_PLANET);
		planeDown->SetModelScale(Vector3(1950.0f, 1950.0f, 1.0f));
		//sun->SetModelScale(Vector3(10.0f, 10.0f, 10.0f));
		planeDown->SetTransform(Matrix4::Rotation(180.0f, Vector3(0, 0, 1)) * Matrix4::Translation(Vector3(0.0f, 2000.0f, 0.0f)) * Matrix4::Rotation(-90.0f, Vector3(1, 0, 0)));
		AddChild(planeDown);
	}
}

SolarSystem::~SolarSystem()
{
	delete sphereMesh;
}

void SolarSystem::Update(float msec)
{
	//Toggle rotation
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_T))
	{
		setRotateObjects(!getRotateObjects());
	}

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_E))
	{
		//mod = 0.0f;
		sunExploding = !sunExploding;
		if (sunExploding)
		{
			sun->GetMesh()->setType(GL_PATCHES);
		}
		else
		{
			sun->GetMesh()->setType(GL_TRIANGLES);
		}
	}

	//transform = transform * Matrix4::Rotation(msec / 10.0f, Vector3(0, 1, 0));
	float sunRotate = 0.7f;
	float earthOrbitSpeed = 0.2f;
	float planet2OrbitSpeed = 0.3f;
	float planet3OrbitSpeed = 0.4f;
	float moonOrbitSpeed = 1.3f;

	Vector3 sunDir = Vector3(0, 1, 0);

	if (rotateObjects)
	{
		sun->SetTransform(sun->GetTransform() * Matrix4::Rotation(sunRotate, sunDir));
		//sun->SetTransform(sun->GetTransform() * Matrix4::Translation(Vector3(10.1f, 0, 0)));

		//planet->SetTransform(planet->GetTransform() * Matrix4::Translation(Vector3(-300.0f * scale, 0.0f, 0.0f)) *
		//	Matrix4::Rotation(-sunRotate + earthOrbitSpeed, sunDir) * Matrix4::Translation(Vector3(300.0f * scale, 0.0f, 0.0f)));

		//planet2->SetTransform(planet2->GetTransform() * Matrix4::Translation(Vector3(-900.0f * scale, 0.0f, 0.0f)) *
		//	Matrix4::Rotation(-sunRotate + planet2OrbitSpeed, sunDir) * Matrix4::Translation(Vector3(900.0f * scale, 0.0f, 0.0f)));

		//planet3->SetTransform(planet3->GetTransform() * Matrix4::Translation(Vector3(-600.0f * scale, 0.0f, 0.0f)) *
		//	Matrix4::Rotation(-sunRotate + planet3OrbitSpeed, sunDir) * Matrix4::Translation(Vector3(600.0f * scale, 0.0f, 0.0f)));

		//moon->SetTransform(moon->GetTransform() * Matrix4::Translation(Vector3(-70.0f, 0.0f, 0.0f)) *
		//	Matrix4::Rotation(-earthOrbitSpeed + moonOrbitSpeed, Vector3(0, 1, 0)) * Matrix4::Translation(Vector3(70.0f, 0.0f, 0.0f)));
	}

	RenderObject::Update(msec);
}

Mesh* SolarSystem::createSphereObj()
{
	OBJMesh* m = new OBJMesh();
	//m->LoadOBJMesh(MESHDIR"sphere2.obj");
	//m->LoadOBJMesh(MESHDIR"mySphere.obj");
	//m->LoadOBJMesh(MESHDIR"sphereDLed.obj");
	//m->LoadOBJMesh(MESHDIR"sphere.obj");
	m->LoadOBJMesh(MESHDIR"smoothSphere.obj");
	

	return m;
}

void SolarSystem::initPlanets()
{
	planets[PlanetNames::LAVA].bodyRadius = 40.0f;
	planets[PlanetNames::LAVA].orbitRadius = 600.0f;
	planets[PlanetNames::LAVA].orbitSpeed = 0.2f;
	planets[PlanetNames::LAVA].orbitSpeed = 30.0f;
	planets[PlanetNames::LAVA].initialRotation = 30.0f;
	planets[PlanetNames::LAVA].texture = TEXTUREDIR"earthTile2.jpg";
	planets[PlanetNames::LAVA].moons = 0;

	planets[PlanetNames::EARTH2].bodyRadius = 30.0f;
	planets[PlanetNames::EARTH2].orbitRadius = 900.0f;
	planets[PlanetNames::EARTH2].orbitSpeed = 30.0f;
	planets[PlanetNames::EARTH2].orbitSpeed = 30.0f;
	planets[PlanetNames::EARTH2].initialRotation = 30.0f;
	planets[PlanetNames::EARTH2].texture = TEXTUREDIR"lavaPlanet2.jpg";
	planets[PlanetNames::EARTH2].moons = 1;

	planets[PlanetNames::SAND].bodyRadius = 50.0f;
	planets[PlanetNames::SAND].orbitRadius = 1200.0f;
	planets[PlanetNames::SAND].orbitSpeed = 30.0f;
	planets[PlanetNames::SAND].orbitSpeed = 30.0f;
	planets[PlanetNames::SAND].initialRotation = 30.0f;
	planets[PlanetNames::SAND].texture = TEXTUREDIR"Barren Reds.jpg";
	planets[PlanetNames::SAND].moons = 3;

	planets[PlanetNames::ROCK].bodyRadius = 35.0f;
	planets[PlanetNames::ROCK].orbitRadius = 1700.0f;
	planets[PlanetNames::ROCK].orbitSpeed = 30.0f;
	planets[PlanetNames::ROCK].orbitSpeed = 30.0f;
	planets[PlanetNames::ROCK].initialRotation = 30.0f;
	planets[PlanetNames::ROCK].texture = TEXTUREDIR"brick.tga";
	planets[PlanetNames::ROCK].moons = 1;

	planets[PlanetNames::ICE].bodyRadius = 70.0f;
	planets[PlanetNames::ICE].orbitRadius = 2000.0f;
	planets[PlanetNames::ICE].orbitSpeed = 30.0f;
	planets[PlanetNames::ICE].orbitSpeed = 30.0f;
	planets[PlanetNames::ICE].initialRotation = 30.0f;
	planets[PlanetNames::ICE].texture = TEXTUREDIR"water05.jpg";
	planets[PlanetNames::ICE].moons = 0;

	planets[PlanetNames::GAS].bodyRadius = 100.0f;
	planets[PlanetNames::GAS].orbitRadius = 2400.0f;
	planets[PlanetNames::GAS].orbitSpeed = 30.0f;
	planets[PlanetNames::GAS].colour.w = 0.5f;
	planets[PlanetNames::GAS].orbitSpeed = 30.0f;
	planets[PlanetNames::GAS].initialRotation = 30.0f;
	planets[PlanetNames::GAS].texture = TEXTUREDIR"saturnmap.jpg";
	planets[PlanetNames::GAS].moons = 2;


	//Populate object information
	//for (int i = 0; i < PlanetNames::NUM_PLANETS; ++i)
	//{
	//	RenderObjects[i] = new RenderObject(new Mesh(*sphereMesh), objectColours[i], types[i]);
	//	RenderObjects[i]->SetModelScale(Vector3(modelScales[i], modelScales[i], modelScales[i]));
	//	RenderObjects[i]->SetTransform(Matrix4::Translation(Vector3(orbitRadii[i], 0.0f, 0.0f)));
	//}

	//float modelScales[NUM_NAMES] = { 100.0f, 20.0f, 40.0f, 10.0f };
	//float orbitRadii[NUM_NAMES] = { 0.0f, 300.0f, 600.0f, 70.0f };
	//float orbitSpeeds[NUM_NAMES] = { 0.0f, 0.3f, 0.5f, 1.3f };
	//float rotateSpeeds[NUM_NAMES] = { 1.0f, 0.5f, 0.7f, 0.3f };
	//RenderType types[NUM_NAMES] = { RenderType::TYPE_SUN, RenderType::TYPE_PLANET, RenderType::TYPE_PLANET, RenderType::TYPE_MOON };

	//Vector4 objectColours[NUM_NAMES] = { Vector4(0.9f, 0.7f, 0.3f, 1.0f),  Vector4(0.2f, 0.7f, 0.9f, 1.0f),
	//	Vector4(0.2f, 0.9f, 0.7f, 1.0f), Vector4(0.4f, 0.4f, 0.4f, 1.0f) };
}

void SolarSystem::SetTextureRepeating(GLuint target, bool repeating) 
{
	glBindTexture(GL_TEXTURE_2D, target);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeating ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeating ? GL_REPEAT : GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);
}