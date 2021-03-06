#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{
	camera = new Camera(0.0f, 0.0f, Vector3(RAW_WIDTH * HEIGHTMAP_X / 2.0f, 500.0f, RAW_HEIGHT * HEIGHTMAP_Z));
	heightMap = new HeightMap(TEXTUREDIR"terrain.raw");
	currentShader = new Shader(SHADERDIR"Tutorials/bumpVertex.glsl", SHADERDIR"Tutorials/bumpFragment.glsl");

	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	//heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"brick.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	//heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"brickDOT3.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	lights[0] = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X * 0.25f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z * 0.25f)),
		Vector4(1, 1, 1, 1), (RAW_WIDTH * HEIGHTMAP_X) / 2.0f);
	lights[1] = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X * 0.25f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z * 0.75f)),
		Vector4(1, 0, 0, 1), (RAW_WIDTH * HEIGHTMAP_X) / 2.0f);
	lights[2] = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X * 0.75f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z * 0.25f)),
		Vector4(0, 1, 0, 1), (RAW_WIDTH * HEIGHTMAP_X) / 2.0f);
	lights[3] = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X * 0.75f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z * 0.75f)),
		Vector4(0, 0, 1, 1), (RAW_WIDTH * HEIGHTMAP_X) / 2.0f);

	if (!currentShader->LinkProgram() || !heightMap->GetTexture() || !heightMap->GetBumpMap())
	{
		return;
	}

	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	init = true;
}

Renderer::~Renderer(void)
{
	delete camera;
	delete heightMap;

	for (int i = 0; i < numLights; ++i)
	{
		delete lights[i];
	}
}

void Renderer::RenderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(currentShader->GetProgram());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

	UpdateShaderMatrices();
	for (int i = 0; i < 4; ++i)
	{
		SetShaderLights(lights);
	}

	heightMap->Draw();

	glUseProgram(0);
	SwapBuffers();
}

void Renderer::UpdateScene(float msec)
{
	updateLights();
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "time"), msec);
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
}

void Renderer::SetShaderLights(Light* lights[numLights])
{
	Vector3 positions[numLights];
	for (int i = 0; i < numLights; ++i)
	{
		positions[i] = lights[i]->GetPosition();
	}

	Vector4 colours[numLights];
	for (int i = 0; i < numLights; ++i)
	{
		colours[i] = lights[i]->GetColour();
	}

	float radii[numLights];
	for (int i = 0; i < numLights; ++i)
	{
		radii[i] = lights[i]->GetRadius();
	}

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "lightPos"), 4, (float*)&positions);
	glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "lightColour"), 4, (float*)&colours);
	glUniform1fv(glGetUniformLocation(currentShader->GetProgram(), "lightRadius"), 4, radii);
}

void Renderer::updateLights()
{
	for (int i = 0; i < 4; ++i)
	{
		timeMods[i]++;

		if (timeMods[i] > 25500)
		{
			timeMods[i] = 0;
		}
	}

	float colourVal = sin((timeMods[0] % 255) * 0.05f);
	if (colourVal < 0.3f)
	{
		colourVal = 0.3f;
	}

	lights[0]->SetColour(Vector4(colourVal, colourVal, colourVal, 1.0f));

	colourVal = sin((timeMods[1] % 255) * 0.05f);
	if (colourVal < 0.3f)
	{
		colourVal = 0.3f;
	}
	lights[1]->SetR(colourVal);

	colourVal = sin((timeMods[2] % 255) * 0.05f);
	if (colourVal < 0.3f)
	{
		colourVal = 0.3f;
	}
	lights[2]->SetG(colourVal);

	colourVal = sin((timeMods[3] % 255) * 0.05f);
	if (colourVal < 0.3f)
	{
		colourVal = 0.3f;
	}
	lights[3]->SetB(colourVal);
}
