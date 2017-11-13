#pragma once

#include "../nclgl/OBJMesh.h"
#include "SolarObject.h"
class SolarSystem : public SceneNode
{
public:
	SolarSystem(void);
	~SolarSystem(void);

	virtual void Update(float msec);

	Mesh* createSphereObj();

	SceneNode* getSun() const { return sun; }
	SceneNode* getPlanet() const { return planet; }
	SceneNode* getPlanet2() const { return planet2; }
	SceneNode* getMoon() const { return moon; }

private:
	Mesh* root = NULL;
	SolarObject* sun;
	SolarObject* planet;
	SolarObject* planet2;
	SolarObject* ring;
	SolarObject* moon;
};