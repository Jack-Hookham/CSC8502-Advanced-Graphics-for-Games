#pragma once

#include "../nclgl/OBJMesh.h"
#include "../nclgl/SceneNode.h"

class SolarSystem : public SceneNode
{
public:
	SolarSystem(void);
	~SolarSystem(void);

	virtual void update(float msec);

	static void createSolarSystem();
	static void deleteSolarSystem();

	SceneNode* getSun() const { return sun; }
	SceneNode* getPlanet() const { return planet; }
	SceneNode* getMoon() const { return moon; }

private:
	static Mesh* sphere;
	SceneNode* sun;
	SceneNode* planet;
	SceneNode* moon;
};