#pragma once

#include "../nclgl/OBJMesh.h"
#include "RenderObject.h"

struct Moon
{
	float bodyRadius = 10.0f;
	float orbitRadius = 100.0f;
	float orbitSpeed = 1.0f;
	float rotationSpeed = 1.0f;
	float initialRotation = 0.0f;
	Vector4 colour = Vector4(0.0, 1.0, 1.0, 1.0);
	char* texture;
};

struct Planet
{
	float bodyRadius = 50.0f;
	float orbitRadius = 500.0f;
	float orbitSpeed = 1.0f;
	float rotationSpeed = 1.0f;
	float initialRotation = 0.0f;
	Vector4 colour = Vector4(0.0, 1.0, 1.0, 1.0);
	char* texture;
	std::vector<Moon> moons;
};

enum PlanetNames
{
	LAVA,
	EARTH2,
	SAND,
	RED,
	GREEN,
	ICE,
	BLUE,
	NUM_PLANETS
};

class SolarSystem : public RenderObject
{
public:
	SolarSystem();
	~SolarSystem(void);

	virtual void Update(float msec);

	Mesh* createSphereObj();

	RenderObject* getSun() const { return sun; }

	const bool getRotateObjects() const { return rotateObjects; }
	void setRotateObjects(const bool v) { rotateObjects = v; }

private:
	void initPlanets();

	void SetTextureRepeating(GLuint target, bool state);

	Mesh* sphereMesh;
	RenderObject* sun;

	Planet planets[NUM_PLANETS];

	//Whether the planets/moons rotation is turned on
	bool rotateObjects = true;

	float scale = 1.0f;

	//Sun data
	float sunRadius = 300.0f;
	float sunRotationSpeed = 0.7f;
};