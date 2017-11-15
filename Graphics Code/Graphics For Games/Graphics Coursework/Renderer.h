#pragma once

#include <iomanip>
#include <sstream>

#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/SceneNode.h"
#include "SolarSystem.h"
#include "TextMesh.h"

#define SHADOWSIZE 2048

enum SceneID
{
	SPACE,
	SCENE2,
	SCENE3,
	NUM_SCENES
};

class Renderer : public OGLRenderer
{
public:
	Renderer(Window &parent);
	virtual ~Renderer(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();

	void setScene(SceneID n) { sceneID = n; }
	SceneID getScene() { return sceneID; }

protected:
	void drawText();
	void DrawSkybox();

	void DrawShadowScene();
	void DrawCombinedScene();

	void compileShaders();

	void DrawNode(SolarObject* n);

	SolarSystem* ss;
	Light* sunLight;

	Camera* camera;

	bool showInfo = true;
	float fps = 0.0f;

	Shader* solarShader;
	Shader* sunShader;
	Shader* skyboxShader;
	Shader* shadowShader;

	Matrix4 shadowMatrix;

	GLuint shadowTex;
	GLuint shadowFBO;

	Mesh* quad;
	GLuint spaceMap;

	Shader* textShader; 
	Font*	basicFont;

	SceneID sceneID = SceneID::SPACE;
};