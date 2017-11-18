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

	void setScene(int n) { sceneID = n; }
	int getScene() { return sceneID; }

	void DrawText(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);

protected:
	void DrawInfo();
	void DrawSkybox();

	void DrawShadowScene();
	void DrawCombinedScene();

	void compileShaders();

	void DrawNode(RenderObject* n);

	int sceneID = SceneID::SPACE;
	RenderObject* currentScene;
	SolarSystem* ss;

	Light* sunLight;

	Camera* camera;

	bool showInfo = true;
	float fps = 0.0f;

	Shader* satelliteShader;
	Shader* sunShader;
	Shader* blackHoleShader;
	Shader* skyboxShader;
	Shader* shadowShader;

	Matrix4 defaultProjMatrix;
	Matrix4 shadowMatrix;

	GLuint shadowTex;
	GLuint shadowFBO;

	Mesh* quad;
	GLuint spaceMap;

	Shader* textShader; 
	Font*	basicFont;

	float mod;

	Matrix4 shadowMapRotations[6] =
	{
		Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3( 1,  0,  0), Vector3(0, -1, 0)),	//+X
		Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3(-1,  0,  0), Vector3(0, -1, 0)),	//-X
		Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3( 0,  1,  0), Vector3(-1, 0, 0)),	//+Y
		Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3( 0, -1,  0), Vector3(-1, 0, 0)),	//-Y
		Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3( 0,  0,  1), Vector3(0, -1, 0)),	//+Z
		Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3( 0,  0, -1), Vector3(0, -1, 0))	//-Z
	};
};