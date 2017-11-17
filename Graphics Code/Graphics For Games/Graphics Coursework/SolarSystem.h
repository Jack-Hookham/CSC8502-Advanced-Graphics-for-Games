#pragma once

#include "../nclgl/OBJMesh.h"
#include "RenderObject.h"

enum SolarNames
{
	SUN,
	//MERCURY,
	//VENUS,
	EARTH,
	//MARS,
	//JUPITER,
	SATURN,
	//URANUS,
	//NEPTUNE,
	//PLUTO,
	MOON,
	NUM_NAMES
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

private:
	Mesh* root = NULL;
	RenderObject* sun;
	RenderObject* planet;
	RenderObject* planet2;
	RenderObject* planet3;
	RenderObject* moon;

	RenderObject* RenderObjects[NUM_NAMES];
	float modelScales[NUM_NAMES] = { 100.0f, 20.0f, 40.0f, 10.0f };
	float orbitRadii[NUM_NAMES] = { 0.0f, 300.0f, 600.0f, 70.0f };
	float orbitSpeeds[NUM_NAMES] = { 0.0f, 0.3f, 0.5f, 1.3f };
	float rotateSpeeds[NUM_NAMES] = { 1.0f, 0.5f, 0.7f, 0.3f };
	RenderType types[NUM_NAMES] = { RenderType::TYPE_SUN, RenderType::TYPE_PLANET, RenderType::TYPE_PLANET, RenderType::TYPE_MOON };

	Vector4 objectColours[NUM_NAMES] = { Vector4(0.9f, 0.7f, 0.3f, 1.0f),  Vector4(0.2f, 0.7f, 0.9f, 1.0f), 
		Vector4(0.2f, 0.9f, 0.7f, 1.0f), Vector4(0.4f, 0.4f, 0.4f, 1.0f) };

	bool rotateObjects = false;

	bool debugShadows = false;
	bool shadowShapes = false;
	float scale = 1.0f;
};