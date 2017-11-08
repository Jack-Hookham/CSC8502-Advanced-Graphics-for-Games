#pragma once

#include <cmath>

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
	int timeMods[4] = { 0, 1000, 3000, 2000 };
	Mesh* heightMap;
	Camera* camera;

	Light* lights[numLights];

	void updateLights();
};