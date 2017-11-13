#pragma once

#include "../nclgl/OBJMesh.h"
#include "../nclgl/SceneNode.h"

class SolarSystem : public SceneNode
{
public:
	SolarSystem(void);
	~SolarSystem(void);

	virtual void Update(float msec);

	Mesh* createSphereObj();

	SceneNode* getSun() const { return sun; }
	SceneNode* getPlanet() const { return planet; }
	SceneNode* getMoon() const { return moon; }

private:
	Mesh* root = NULL;
	SceneNode* sun;
	SceneNode* planet;
	SceneNode* planet2;
	SceneNode* ring;
	SceneNode* moon;
};