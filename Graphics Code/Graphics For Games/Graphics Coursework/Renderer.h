#pragma once

#include <iomanip>
#include <sstream>

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "SolarSystem.h"
#include "textmesh.h"

class Renderer : public OGLRenderer
{
public:
	Renderer(Window &parent);
	virtual ~Renderer(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();

	void drawText();

protected:
	void DrawNode(SceneNode* n);

	Light* sunLight;

	SceneNode* root;
	Camera* camera;

	bool showInfo = true;
	float fps = 0.0f;

	Shader* ringShader;

	Shader* textShader; 
	Font*	basicFont;
};