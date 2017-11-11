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

private:
	static Mesh* sphere;
	SceneNode* sun;
	SceneNode* planet;
	SceneNode* moon;
};