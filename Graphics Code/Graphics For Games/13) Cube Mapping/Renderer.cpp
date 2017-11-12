#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{
	//heightMap = new HeightMap(HEIGHTMAPSDIR"grand-canyon.data");
	heightMap = new HeightMap(HEIGHTMAPSDIR"grandCanyon1080_02.data", 1080, 1080, 16.0f, 16.0f, 8.0f, 1.0f / 16.0f, 1.0f / 16.0f);
	quad = Mesh::GenerateQuad();
	camera = new Camera();
	camera->SetPosition(Vector3(heightMap->getRawWidth() * heightMap->getHeightMapX() / 2.0f, 500.0f, 
		heightMap->getRawWidth() * heightMap->getHeightMapX()));
	//heightMap = new HeightMap(TEXTUREDIR"river257.png");

	//light = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)), Vector4(0.9f, 0.9f, 1.0f, 1),
	//	RAW_WIDTH * HEIGHTMAP_X / 2.0f);
	light = new Light(Vector3((heightMap->getRawHeight() * heightMap->getHeightMapX() * 100.0f), 1000000.0f,
		(heightMap->getRawHeight() * heightMap->getHeightMapX() * -60.0f)), Vector4(1.0f, 0.7f, 0.4f, 1),
		heightMap->getRawWidth() * heightMap->getHeightMapX() * 100000.0f);

	//reflectShader = new Shader(SHADERDIR"Tutorials/bumpVertex.glsl", SHADERDIR"Tutorials/bumpFragment.glsl");
	reflectShader = new Shader(SHADERDIR"Tutorials/bumpVertex.glsl", SHADERDIR"Tutorials/reflectFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"Tutorials/skyboxVertex.glsl", SHADERDIR"Tutorials/skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR"Tutorials/bumpVertex.glsl", SHADERDIR"Tutorials/bumpFragment.glsl");

	if (!reflectShader->LinkProgram() || !skyboxShader->LinkProgram() || !lightShader->LinkProgram())
	{
		return;
	}

	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water2.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	quad->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"waterBumpMap1.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg", TEXTUREDIR"rusted_up.jpg",
		TEXTUREDIR"rusted_down.jpg", TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	if (!cubeMap || !quad->GetTexture() || !heightMap->GetTexture() || !heightMap->GetBumpMap())
	{
		return;
	}

	SetTextureRepeating(quad->GetTexture(), true);
	SetTextureRepeating(quad->GetBumpMap(), true);
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);

	waterRotate = 0.0f;

	projMatrix = Matrix4::Perspective(1.0f, 50000.0f, (float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	init = true;
}

Renderer::~Renderer(void)
{
	delete camera;
	delete heightMap;
	delete quad;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;
	currentShader = 0;
}

void Renderer::UpdateScene(float msec)
{
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	waterRotate += msec / 1000.0f;
}

void Renderer::RenderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSkybox();
	DrawHeightMap();
	DrawWater();

	SwapBuffers();
}

void Renderer::DrawHeightMap()
{
	SetCurrentShader(lightShader);
	SetShaderLight(*light);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	heightMap->Draw();

	glUseProgram(0);
}

void Renderer::DrawWater()
{
	SetCurrentShader(reflectShader);
	SetShaderLight(*light);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, heightMap->GetBumpMap());

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	float heightX = (heightMap->getRawWidth() * heightMap->getHeightMapX() / 2.0f);
	float heightY = 256 * heightMap->getHeightMapY() / 5.0f;
	float heightZ = (heightMap->getRawHeight() * heightMap->getHeightMapZ() / 2.0f);

	modelMatrix =
		Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
		Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
}

void Renderer::DrawSkybox()
{
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}