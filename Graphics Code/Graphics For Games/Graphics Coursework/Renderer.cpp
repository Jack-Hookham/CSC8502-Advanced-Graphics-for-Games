#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)
{
	sunLight = new Light(Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 10000.0f);
	//SolarSystem::createSphereObj();
	camera = new Camera();

	quad = Mesh::GenerateQuad();
	spaceMap = SOIL_load_OGL_cubemap(TEXTUREDIR"galaxy_west.bmp", TEXTUREDIR"galaxy_east.bmp", TEXTUREDIR"galaxy_up.bmp",
		TEXTUREDIR"galaxy_down.bmp", TEXTUREDIR"galaxy_south.bmp", TEXTUREDIR"galaxy_north.bmp",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	//spaceMap = SOIL_load_OGL_cubemap(TEXTUREDIR"rusted_west.jpg", TEXTUREDIR"rusted_east.jpg", TEXTUREDIR"rusted_up.jpg",
	//	TEXTUREDIR"rusted_down.jpg", TEXTUREDIR"rusted_south.jpg", TEXTUREDIR"rusted_north.jpg",
	//	SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	currentShader = NULL;
	compileShaders();

	SetCurrentShader(solarShader);

	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	camera->SetPosition(Vector3(0, 30.0f, 500.0f));

	ss = new SolarSystem();

	ss->getPlanet()->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"earthTile2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	ss->getPlanet2()->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water05.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	ss->getPlanet3()->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"lavaPlanet2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	ss->getSun()->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"TileFire.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	ss->getMoon()->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	//ss->getMoon()->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"sunmap.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	// | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	if (!ss->getSun()->GetMesh()->GetTexture() ||
		!ss->getPlanet()->GetMesh()->GetTexture() ||
		!ss->getPlanet2()->GetMesh()->GetTexture() ||
		!ss->getMoon()->GetMesh()->GetTexture() ||
		!spaceMap)
	{
		return;
	}

	SetTextureRepeating(ss->getPlanet()->GetMesh()->GetTexture(), true);
	SetTextureRepeating(ss->getPlanet2()->GetMesh()->GetTexture(), true);
	SetTextureRepeating(ss->getPlanet3()->GetMesh()->GetTexture(), true);
	SetTextureRepeating(ss->getMoon()->GetMesh()->GetTexture(), true);
	SetTextureRepeating(ss->getSun()->GetMesh()->GetTexture(), true);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_DEPTH_TEST);
	init = true;
}

Renderer::~Renderer(void)
{
	delete ss;
	delete camera;
	delete sunLight;
	delete basicFont;

	delete solarShader;
	delete ringShader;
	delete sunShader;
	delete textShader;
	delete skyboxShader;
	currentShader = NULL;

	delete quad;

	//SolarSystem::deleteSphereObj();
}

void Renderer::UpdateScene(float msec)
{
	//Recompile shaders if R pressed
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_R))
	{
		compileShaders();
	}

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_T))
	{
		ss->setRotateObjects(!ss->getRotateObjects());
	}

	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	ss->Update(msec);
}

void Renderer::compileShaders()
{
	solarShader = new Shader(SHADERDIR"CW/solarVertex.glsl", SHADERDIR"CW/solarFragment.glsl");
	//Sun uses a different shader as it doesn't need lighting
	sunShader = new Shader(SHADERDIR"CW/sunVertex.glsl", SHADERDIR"CW/sunFragment.glsl");
	ringShader = new Shader(SHADERDIR"CW/ringVertex.glsl", SHADERDIR"CW/ringFragment.glsl");
	textShader = new Shader(SHADERDIR"CW/texturedVertex.glsl", SHADERDIR"CW/texturedFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"CW/skyboxVertex.glsl", SHADERDIR"CW/skyboxFragment.glsl");

	if (!solarShader->LinkProgram() ||
		!ringShader->LinkProgram() ||
		!textShader->LinkProgram() ||
		!sunShader->LinkProgram() ||
		!skyboxShader->LinkProgram())
	{
		return;
	}
	SetCurrentShader(solarShader);
}

void Renderer::RenderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//glUseProgram(currentShader->GetProgram());

	SetShaderLight(*sunLight);

	UpdateShaderMatrices();

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, (float*)&textureMatrix);

	//glActiveTexture(GL_TEXTURE0);

	DrawSkybox();
	DrawNode(ss);
	glUseProgram(0);


	drawText();

	SwapBuffers();
}

void Renderer::drawText()
{
	glEnable(GL_BLEND);

	//glUseProgram(textShader->GetProgram());
	SetCurrentShader(textShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), basicFont->texture);

	//DrawText("This is orthographic text!", Vector3(0, 0, 0), 16.0f);

	//std::ostringstream oss;
	//oss << std::fixed << std::setprecision(2) << fps;
	//DrawText(oss.str(), Vector3(50.0f, 50.0f, 0), 16.0f);

	glUseProgram(0);
	glDisable(GL_BLEND);
}

void Renderer::DrawNode(SolarObject* n)
{
	if (n->getType() == SolarType::TYPE_SUN)
	{
		SetCurrentShader(sunShader);
		//SetCurrentShader(solarShader);
	}
	else if (n->getType() == SolarType::TYPE_PLANET || n->getType() == SolarType::TYPE_MOON)
	{
		SetCurrentShader(solarShader);
	}

	if (n->GetMesh())
	{
		UpdateShaderMatrices();
		Matrix4 transform = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)&transform);
		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)n->GetMesh()->GetTexture());
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

		n->Draw();
	}

	for (vector<SolarObject*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i)
	{
		DrawNode(*i);
	}
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