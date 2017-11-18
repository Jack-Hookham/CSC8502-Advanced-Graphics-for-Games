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
	SolarSystem(void);
	~SolarSystem(void);

	virtual void Update(float msec);

	Mesh* createSphereObj();

	RenderObject* getSun() const { return sun; }
	RenderObject* getPlanet() const { return planet; }
	RenderObject* getPlanet2() const { return planet2; }
	RenderObject* getPlanet3() const { return planet3; }
	RenderObject* getMoon() const { return moon; }

	const bool getRotateObjects() const { return rotateObjects; }
	void setRotateObjects(const bool v) { rotateObjects = v; }

	bool getExploding() { return sunExploding; }
	void setExploding(const bool value) { sunExploding = value; }

private:
	void initPlanets();

	void SetTextureRepeating(GLuint target, bool state);

	Mesh* sphereMesh;
	RenderObject* sun;
	RenderObject* planet;
	RenderObject* planet2;
	RenderObject* planet3;
	RenderObject* moon;

	Planet planets[NUM_PLANETS];

	bool rotateObjects = false;

	float scale = 1.0f;

	bool sunExploding = false;

	//Sun data
	float sunRadius = 300.0f;
	float sunRotationSpeed = 0.7f;
};