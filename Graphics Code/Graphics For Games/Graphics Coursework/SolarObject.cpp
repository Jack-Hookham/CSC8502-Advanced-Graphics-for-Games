#include "SolarObject.h"

SolarObject::SolarObject(Mesh* m, Vector4 colour, SolarType t)
{
	type = t;
	this->mesh = m;
	this->colour = colour;
	parent = NULL;
	modelScale = Vector3(1, 1, 1);
	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
}

SolarObject::~SolarObject(void)
{
	delete mesh;
	for (unsigned int i = 0; i < children.size(); ++i)
	{
		delete children[i];
	}
}

void SolarObject::AddChild(SolarObject* s)
{
	children.push_back(s);
	s->parent = this;
}

void SolarObject::Update(float msec)
{
	//This node has a parent...
	if (parent)
	{
		worldTransform = parent->worldTransform * transform;
	}
	//Root node, world transform is local transform
	else
	{
		worldTransform = transform;
	}

	for (vector<SolarObject*>::iterator i = children.begin(); i != children.end(); ++i)
	{
		(*i)->Update(msec);
	}
}

void SolarObject::Draw()
{
	if (mesh)
	{
		mesh->Draw();
	}
}
