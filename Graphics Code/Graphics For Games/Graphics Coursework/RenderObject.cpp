#include "RenderObject.h"

RenderObject::RenderObject(Mesh* m, Vector4 colour, RenderType t)
{
	type = t;
	this->mesh = m;
	this->colour = colour;
	parent = NULL;
	modelScale = Vector3(1, 1, 1);
	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
}

RenderObject::~RenderObject(void)
{
	delete mesh;
	for (unsigned int i = 0; i < children.size(); ++i)
	{
		delete children[i];
	}
}

void RenderObject::AddChild(RenderObject* s)
{
	children.push_back(s);
	s->parent = this;
}

void RenderObject::Update(float msec)
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

	worldTransform = worldTransform * selfTransform;

	for (vector<RenderObject*>::iterator i = children.begin(); i != children.end(); ++i)
	{
		(*i)->Update(msec);
	}
}

void RenderObject::Draw()
{
	if (mesh)
	{
		mesh->Draw();
	}
}

void RenderObject::DrawPatches()
{
	if (mesh)
	{
		mesh->DrawPatches();
	}
}
