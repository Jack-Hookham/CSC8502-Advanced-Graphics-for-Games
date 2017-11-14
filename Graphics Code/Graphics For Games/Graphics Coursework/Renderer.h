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
	void DrawSkybox();

protected:
	void compileShaders();

	void DrawNode(SolarObject* n);

	Light* sunLight;

	SolarObject* root;
	Camera* camera;

	bool showInfo = true;
	float fps = 0.0f;

	Shader* solarShader;
	Shader* ringShader;
	Shader* sunShader;
	Shader* skyboxShader;

	Mesh* quad;
	GLuint spaceMap;

	Shader* textShader; 
	Font*	basicFont;
};