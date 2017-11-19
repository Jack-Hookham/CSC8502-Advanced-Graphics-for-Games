#pragma once

#include <iomanip>
#include <sstream>

#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/HeightMap.h"
#include "SolarSystem.h"
#include "Volcano.h"
#include "Mountains.h"
#include "TextMesh.h"

#define SHADOWSIZE 2048

enum SceneID
{
	SOLAR_SCENE,
	VOLCANO_SCENE,
	MOUTAIN_SCENE,
	NUM_SCENES
};

class Renderer : public OGLRenderer
{
public:
	Renderer(Window &parent);
	virtual ~Renderer(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();

	void setScene(int n) 
	{ 
		sceneID = n;
		currentScene = scenes[sceneID];
	}

	int getScene() { return sceneID; }

	void DrawText(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);

protected:
	float sceneTimer = 0.0f;

	void DrawInfo();
	void DrawSkybox();

	void compileShaders();

	void DrawNode(RenderObject* n);

	int sceneID = SceneID::SOLAR_SCENE;
	RenderObject* currentScene;
	RenderObject* scenes[SceneID::NUM_SCENES];

	SolarSystem* ss;
	Volcano* volcano;
	Mountains* mountains;

	Light* sunLight;

	Camera* camera;

	bool showInfo = true;
	float fps = 0.0f;

	Shader* skyboxShader;

	Matrix4 defaultProjMatrix;

	Mesh* quad;

	Shader* textShader; 
	Font*	basicFont;

	//-----------------------Solar System Scene---------------------
	void DrawShadowScene();
	void DrawCombinedScene();

	void initShadowMap();

	Matrix4 shadowMatrix;

	GLuint shadowTex;
	GLuint shadowFBO;

	GLuint spaceMap;

	Shader* shadowShader;
	Shader* satelliteShader;
	Shader* sunShader;
	Shader* blackHoleShader;

	Matrix4 shadowMapRotations[6] =
	{
		Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3( 1,  0,  0), Vector3( 0, -1, 0)),	//+X
		Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3(-1,  0,  0), Vector3( 0, -1, 0)),	//-X
		Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3( 0,  1,  0), Vector3(-1,  0, 0)),	//+Y
		Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3( 0, -1,  0), Vector3(-1,  0, 0)),	//-Y
		Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3( 0,  0,  1), Vector3( 0, -1, 0)),	//+Z
		Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3( 0,  0, -1), Vector3( 0, -1, 0))		//-Z
	};

	//-----------------------Volcano Scene-----------------------------

	//-----------------------Mountains Scene---------------------------
	void DrawHeightMap();
	void DrawWater();

	Shader* mountainsLightShader;
	Shader* reflectShader;

	Light* mountainsLight;

	HeightMap* mountainsHeightMap;

	float waterRotate;
};