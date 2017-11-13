#include "SolarObject.h"

SolarObject::SolarObject(Mesh* m, Vector4 colour, SolarType t)
{
	type = t;
	//SceneNode::SceneNode(m, colour);

	this->mesh = m;
	this->colour = colour;
	parent = NULL;
	modelScale = Vector3(1, 1, 1);
	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
}

SolarObject::~SolarObject()
{
	SceneNode::~SceneNode();
}
