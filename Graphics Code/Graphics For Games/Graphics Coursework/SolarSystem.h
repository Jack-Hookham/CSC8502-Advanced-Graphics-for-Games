#pragma once

#include "../nclgl/OBJMesh.h"
#include "../nclgl/SceneNode.h"

class SolarSystem : public SceneNode
{
public:
	SolarSystem(void);
	~SolarSystem(void);

	virtual void Update(float msec);

	static void createSphereObj();
	static void deleteSphereObj();

	SceneNode* getSun() const { return sun; }
	SceneNode* getPlanet() const { return planet; }
	SceneNode* getMoon() const { return moon; }

private:
	static Mesh* sphere;
	SceneNode* sun;
	SceneNode* planet;
	SceneNode* moon;
};