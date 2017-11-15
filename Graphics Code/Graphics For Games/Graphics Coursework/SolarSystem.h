#pragma once

#include "../nclgl/OBJMesh.h"
#include "SolarObject.h"

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

class SolarSystem : public SolarObject
{
public:
	SolarSystem(void);
	~SolarSystem(void);

	virtual void Update(float msec);

	Mesh* createSphereObj();

	SolarObject* getSun() const { return sun; }
	SolarObject* getPlanet() const { return planet; }
	SolarObject* getPlanet2() const { return planet2; }
	SolarObject* getPlanet3() const { return planet3; }
	SolarObject* getMoon() const { return moon; }

	const bool getRotateObjects() const { return rotateObjects; }
	void setRotateObjects(const bool v) { rotateObjects = v; }

private:
	Mesh* root = NULL;
	SolarObject* sun;
	SolarObject* planet;
	SolarObject* planet2;
	SolarObject* planet3;
	SolarObject* moon;

	SolarObject* solarObjects[NUM_NAMES];
	float modelScales[NUM_NAMES] = { 100.0f, 20.0f, 40.0f, 10.0f };
	float orbitRadii[NUM_NAMES] = { 0.0f, 300.0f, 600.0f, 70.0f };
	float orbitSpeeds[NUM_NAMES] = { 0.0f, 0.3f, 0.5f, 1.3f };
	float rotateSpeeds[NUM_NAMES] = { 1.0f, 0.5f, 0.7f, 0.3f };
	SolarType types[NUM_NAMES] = { SolarType::TYPE_SUN, SolarType::TYPE_PLANET, SolarType::TYPE_PLANET, SolarType::TYPE_MOON };

	Vector4 objectColours[NUM_NAMES] = { Vector4(0.9f, 0.7f, 0.3f, 1.0f),  Vector4(0.2f, 0.7f, 0.9f, 1.0f), 
		Vector4(0.2f, 0.9f, 0.7f, 1.0f), Vector4(0.4f, 0.4f, 0.4f, 1.0f) };

	bool rotateObjects = false;
};