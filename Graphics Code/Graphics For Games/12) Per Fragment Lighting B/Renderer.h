#pragma once

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/HeightMap.h"

#define numLights 4

class Renderer : public OGLRenderer
{
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void SetShaderLights(Light* lights[numLights]);

protected:
	Mesh* heightMap;
	Camera* camera;

	Light* lights[numLights];
};