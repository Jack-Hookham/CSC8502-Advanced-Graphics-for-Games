#pragma once

#define WEEK_2_CODE

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/HeightMap.h"

#define POST_PASSES 10

class Renderer : public OGLRenderer
{
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

protected:
	void PresentScene();
	void DrawPostProcess();
	void DrawScene();

	Shader* sceneShader;
	Shader* processShader;

	Mesh* quad;
	HeightMap* heightMap;
	Camera* camera;

	GLuint bufferFBO;
	GLuint processFBO;
	GLuint bufferColourTex[2];
	GLuint bufferDepthTex;
};