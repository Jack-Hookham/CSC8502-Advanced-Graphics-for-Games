#include "SceneNode.h"

SceneNode::SceneNode(Mesh* m, Vector4 colour)
{
	this->mesh = m;
	this->colour = colour;
	parent = NULL;
	modelScale = Vector3(1, 1, 1);
	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
}

SceneNode::~SceneNode(void)
{
	for (unsigned int i = 0; i < children.size(); ++i)
	{
		delete children[i];
	}
}

void SceneNode::AddChild(SceneNode* s)
{
	children.push_back(s);
	s->parent = this;
}

void SceneNode::Update(float msec)
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

	for (vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i)
	{
		(*i)->Update(msec);
	}
}

void SceneNode::Draw()
{
	if (mesh) 
	{
		mesh->Draw();
	}
}
