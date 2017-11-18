#include "SolarSystem.h"

SolarSystem::SolarSystem()
{
	//Initialise planet data
	initPlanets();

	//Mesh* sphereMesh = createSphereObj();

	//Populate object information
	sun = new RenderObject(createSphereObj(), Vector4(0.9f, 0.7f, 0.3f, 1.0f), RenderType::TYPE_SUN);
	sun->SetModelScale(Vector3(sunRadius * scale));
	sun->SetTransform(Matrix4::Translation(Vector3(0.0f, 0.0f, 0.0f)));
	sun->SetBoundingRadius(sunRadius * scale);
	sun->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"TileFire.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if (!sun->GetMesh()->GetTexture())
	{
		return;
	}

	SetTextureRepeating(sun->GetMesh()->GetTexture(), true);

	AddChild(sun);

	for (int i = 0; i < PlanetNames::NUM_PLANETS; ++i)
	{
		RenderObject* o = new RenderObject(createSphereObj(), planets[i].colour, RenderType::TYPE_SATELLITE);
		o->SetModelScale(Vector3(planets[i].bodyRadius));
		o->SetTransform(Matrix4::Rotation(planets[i].initialRotation, Vector3(0.0f, 1.0f, 0.0f)) * Matrix4::Translation(Vector3(planets[i].orbitRadius, 0.0f, 0.0f)));
		o->GetMesh()->SetTexture(SOIL_load_OGL_texture(planets[i].texture, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

		if (!o->GetMesh()->GetTexture())
		{
			return;
		}

		SetTextureRepeating(o->GetMesh()->GetTexture(), true);

		for (int j = 0; j < planets[i].moons.size(); ++j)
		{
			RenderObject* om = new RenderObject(createSphereObj(), Vector4(0.5f, 0.5f, 0.5f, 1.0f), RenderType::TYPE_SATELLITE);
			om->SetModelScale(Vector3(planets[i].moons[j].bodyRadius));
			om->SetTransform(Matrix4::Rotation(planets[i].moons[j].initialRotation, Vector3(0.0f, 1.0f, 0.0f)) * Matrix4::Translation(Vector3(planets[i].moons[j].orbitRadius, 0.0f, 0.0f)));
			om->GetMesh()->SetTexture(SOIL_load_OGL_texture(planets[i].moons[j].texture, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

			if (!om->GetMesh()->GetTexture())
			{
				return;
			}

			SetTextureRepeating(om->GetMesh()->GetTexture(), true);

			o->AddChild(om);
		}
		sun->AddChild(o);
	}
}

SolarSystem::~SolarSystem()
{
	RenderObject::~RenderObject();
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

	Vector3 sunDir = Vector3(0, 1, 0);

	if (rotateObjects)
	{
		sun->SetTransform(sun->GetTransform() * Matrix4::Rotation(sunRotationSpeed, sunDir));

		//Rotate each planet around the sun
		for (int i = 0; i < NUM_PLANETS; ++i)
		{
			RenderObject* planet = sun->GetChild(i);
			planet->SetTransform(planet->GetTransform() * Matrix4::Translation(Vector3(-planets[i].orbitRadius * scale, 0.0f, 0.0f)) *
				Matrix4::Rotation(-sunRotationSpeed + planets[i].orbitSpeed, sunDir) * Matrix4::Translation(Vector3(planets[i].orbitRadius * scale, 0.0f, 0.0f)));

			//Rotate each moon around its planet
			for (int j = 0; j < planets[i].moons.size(); ++j)
			{
				RenderObject* moon = planet->GetChild(j);
				moon->SetTransform(moon->GetTransform() * Matrix4::Translation(Vector3(-planets[i].moons[j].orbitRadius * scale, 0.0f, 0.0f)) *
					Matrix4::Rotation(planets[i].moons[j].orbitSpeed, sunDir) * Matrix4::Translation(Vector3(planets[i].moons[j].orbitRadius * scale, 0.0f, 0.0f)));
			}
		}
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
	int currentPlanet = PlanetNames::LAVA;
	planets[currentPlanet].bodyRadius = 40.0f;
	planets[currentPlanet].orbitRadius = sunRadius + 600.0f;
	planets[currentPlanet].orbitSpeed = 0.27f;
	planets[currentPlanet].initialRotation = 0.0f;
	planets[currentPlanet].texture = TEXTUREDIR"lavaPlanet2.jpg";

	currentPlanet = PlanetNames::EARTH2;
	planets[currentPlanet].bodyRadius = 30.0f;
	planets[currentPlanet].orbitRadius = sunRadius + 1000.0f;
	planets[currentPlanet].orbitSpeed = 0.24f;
	planets[currentPlanet].initialRotation = 40.0f;
	planets[currentPlanet].texture = TEXTUREDIR"earthTile2.jpg";

	Moon moon;
	moon.bodyRadius = 10.0f;
	moon.orbitRadius = planets[currentPlanet].bodyRadius + 70.0f;
	moon.orbitSpeed = 0.2f;
	moon.initialRotation = 10.0f;
	moon.texture = TEXTUREDIR"Barren Reds.jpg";
	planets[currentPlanet].moons.push_back(moon);

	currentPlanet = PlanetNames::SAND;
	planets[currentPlanet].bodyRadius = 50.0f;
	planets[currentPlanet].orbitRadius = sunRadius + 1700.0f;
	planets[currentPlanet].orbitSpeed = 0.29f;
	planets[currentPlanet].initialRotation = 30.0f;
	planets[currentPlanet].texture = TEXTUREDIR"TileSandStone.jpg";

	moon.bodyRadius = 15.0f;
	moon.orbitRadius = planets[currentPlanet].bodyRadius + 50.0f;
	moon.orbitSpeed = 0.22f;
	moon.initialRotation = 10.0f;
	moon.texture = TEXTUREDIR"Barren Reds.jpg";
	planets[currentPlanet].moons.push_back(moon);

	moon.bodyRadius = 10.0f;
	moon.orbitRadius = planets[currentPlanet].bodyRadius + 90.0f;
	moon.orbitSpeed = 0.27f;
	moon.initialRotation = 130.0f;
	moon.texture = TEXTUREDIR"Barren Reds.jpg";
	planets[currentPlanet].moons.push_back(moon);

	moon.bodyRadius = 25.0f;
	moon.orbitRadius = planets[PlanetNames::SAND].bodyRadius + 150.0f;
	moon.orbitSpeed = 0.34f;
	moon.initialRotation = 190.0f;
	moon.texture = TEXTUREDIR"Barren Reds.jpg";
	planets[PlanetNames::SAND].moons.push_back(moon);

	currentPlanet = PlanetNames::RED;
	planets[currentPlanet].bodyRadius = 45.0f;
	planets[currentPlanet].orbitRadius = sunRadius + 2200.0f;
	planets[currentPlanet].orbitSpeed = 0.22f;
	planets[currentPlanet].initialRotation = 185.0f;
	planets[currentPlanet].texture = TEXTUREDIR"RedPlanet.jpg";

	moon.bodyRadius = 18.0f;
	moon.orbitRadius = planets[currentPlanet].bodyRadius + 130.0f;
	moon.orbitSpeed = 0.28f;
	moon.initialRotation = 140.0f;
	moon.texture = TEXTUREDIR"Barren Reds.jpg";
	planets[currentPlanet].moons.push_back(moon);

	currentPlanet = PlanetNames::GREEN;
	planets[currentPlanet].bodyRadius = 40.0f;
	planets[currentPlanet].orbitRadius = sunRadius + 2700.0f;
	planets[currentPlanet].orbitSpeed = 0.23f;
	planets[currentPlanet].initialRotation = 220.0f;
	planets[currentPlanet].texture = TEXTUREDIR"Emerald.jpg";

	moon.bodyRadius = 10.0f;
	moon.orbitRadius = planets[currentPlanet].bodyRadius + 60.0f;
	moon.orbitSpeed = 0.22f;
	moon.initialRotation = 180.0f;
	moon.texture = TEXTUREDIR"Emerald.jpg";
	planets[currentPlanet].moons.push_back(moon);

	moon.bodyRadius = 25.0f;
	moon.orbitRadius = planets[currentPlanet].bodyRadius + 140.0f;
	moon.orbitSpeed = 0.31f;
	moon.initialRotation = 310.0f;
	moon.texture = TEXTUREDIR"Emerald.jpg";
	planets[currentPlanet].moons.push_back(moon);

	currentPlanet = PlanetNames::ICE;
	planets[currentPlanet].bodyRadius = 70.0f;
	planets[currentPlanet].orbitRadius = sunRadius + 3300.0f;
	planets[currentPlanet].orbitSpeed = 0.25f;
	planets[currentPlanet].initialRotation = 200.0f;
	planets[currentPlanet].texture = TEXTUREDIR"water05.jpg";

	currentPlanet = PlanetNames::BLUE;
	planets[currentPlanet].bodyRadius = 100.0f;
	planets[currentPlanet].orbitRadius = sunRadius + 4000.0f;
	planets[currentPlanet].orbitSpeed = 0.19f;
	planets[currentPlanet].initialRotation = 180.0f;
	planets[currentPlanet].texture = TEXTUREDIR"BluePlanet.jpg";

	moon.bodyRadius = 22.0f;
	moon.orbitRadius = planets[currentPlanet].bodyRadius + 90.0f;
	moon.orbitSpeed = 0.27f;
	moon.initialRotation = 160.0f;
	moon.texture = TEXTUREDIR"Barren Reds.jpg";
	planets[currentPlanet].moons.push_back(moon);

	moon.bodyRadius = 35.0f;
	moon.orbitRadius = planets[currentPlanet].bodyRadius + 170.0f;
	moon.orbitSpeed = 0.23f;
	moon.initialRotation = 340.0f;
	moon.texture = TEXTUREDIR"Barren Reds.jpg";
	planets[currentPlanet].moons.push_back(moon);

	moon.bodyRadius = 26.0f;
	moon.orbitRadius = planets[currentPlanet].bodyRadius + 280.0f;
	moon.orbitSpeed = 0.19f;
	moon.initialRotation = 80.0f;
	moon.texture = TEXTUREDIR"water05.jpg";
	planets[currentPlanet].moons.push_back(moon);
}

void SolarSystem::SetTextureRepeating(GLuint target, bool repeating) 
{
	glBindTexture(GL_TEXTURE_2D, target);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeating ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeating ? GL_REPEAT : GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);
}