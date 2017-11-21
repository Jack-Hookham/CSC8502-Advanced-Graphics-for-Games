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
#include "ParticleEmitter.h"

#define SHADOWSIZE 2048
#define POST_PASSES 10

enum SceneID
{
	SOLAR_SCENE,
	VOLCANO_SCENE,
	MOUNTAIN_SCENE,
	NUM_SCENES
};

class Renderer : public OGLRenderer
{
public:
	Renderer(Window &parent);
	virtual ~Renderer(void);

	virtual void UpdateScene(float msec);
	virtual void RenderScene();

	void setScene(int scene);

	int getScene() { return sceneID; }

	void drawText(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);

protected:
	//Total time since program started
	float totalTimer = 0.0f;
	//Total time of current scene
	float sceneTimer = 0.0f;

	//fps timer used to calculate average fps over 1 second
	//gets reset every second
	float fpsTimer = 0.0f;
	//frame count to count the number of frames that have passed in the last second
	int frameCount = 0;
	float fps = 0.0f;

	bool showInfo = true;

	void drawInfo();

	void DrawSkybox();

	void compileShaders();

	void DrawNode(RenderObject* n);

	//Post processing
	void DrawBlur();
	void DrawFinalScene();

	void shakeCamera(const float msec, Camera* camera);

	Mesh* processQuad;
	Shader* blurShader;
	Shader* sceneShader;
	GLuint bufferFBO;
	GLuint processFBO;
	GLuint bufferColourTex[2];
	GLuint bufferDepthTex;

	int sceneID = SceneID::SOLAR_SCENE;

	std::string currentSceneName;
	std::string sceneNames[SceneID::NUM_SCENES] = 
	{
		"Space",
		"Volcano",
		"Mountains"
	};

	RenderObject* currentScene;
	RenderObject* scenes[SceneID::NUM_SCENES];

	Camera* currentCamera;
	Camera* cameras[SceneID::NUM_SCENES];

	GLuint currentSkyMap = 0;
	GLuint skyMaps[SceneID::NUM_SCENES];

	SolarSystem* ss;
	Volcano* volcano;
	Mountains* mountains;

	Light* sunLight;

	Shader* skyboxShader;

	Matrix4 defaultProjMatrix;

	Shader* textShader; 
	Font*	basicFont;

	//Post processing
	float blurFactor = 0.0f;

	bool switchingLeft = false;
	bool switchingRight = false;

	//-----------------------Solar System Scene---------------------

	void DrawShadowScene();
	void DrawCombinedScene();

	void initShadowMap();

	Matrix4 shadowMatrix;

	GLuint shadowTex;
	GLuint shadowFBO;

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

	void DrawVolcanoMap();
	void DrawFloorLava();
	void DrawVolcanoLava();
	void DrawEmitters();
	void SetShaderParticleSize(const float f);

	HeightMap* volcanoHeightMap;
	Shader* volcanoLightShader;
	Shader* lavaShader;

	Light* volcanoLight;

	Mesh* lavaQuad;

	Shader* particleShader;
	ParticleEmitter* lavaEmitter;
	ParticleEmitter* emberEmitter;
	ParticleEmitter* steamEmitter;

	Camera* volcanoCamera2;

	bool volcanoErupting = false;

	//-----------------------Mountains Scene---------------------------

	void DrawMountainMap();
	void DrawWater();

	Shader* mountainsLightShader;
	Shader* reflectShader;
	Shader* mountainSkyboxShader;
	float sunStrength = 0.0f;		//Passed into the mountain skybox shader to adjust its alpha based on day/night
	float minSunX; 
	float maxSunX;

	Vector3 mountainsLightReset;
	Light* mountainsLight;

	HeightMap* mountainsHeightMap;

	Mesh* waterQuad;
};