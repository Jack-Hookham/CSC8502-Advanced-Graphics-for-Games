#pragma once

#include "../nclgl/OBJMesh.h"
#include "../nclgl/SceneNode.h"

class CubeRobot : public SceneNode
{
public:
	CubeRobot(void);
	~CubeRobot(void) {};

	virtual void Update(float msec);

	static void CreateCube();
	static void DeleteCube() { delete cube; }

	SceneNode* getHead() { return head; }

protected:
	static Mesh* cube;
	SceneNode* head;
	SceneNode* leftArm;
	SceneNode* rightArm;
};