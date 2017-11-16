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

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowTex);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, GL_DEPTH_COMPONENT32F, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_DEPTH_COMPONENT32F, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_DEPTH_COMPONENT32F, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_DEPTH_COMPONENT32F, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_DEPTH_COMPONENT32F, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_DEPTH_COMPONENT32F, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glGenFramebuffers(1, &shadowFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
		!ss->getPlanet3()->GetMesh()->GetTexture() ||
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

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
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

	//Toggle rotation
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
	textShader = new Shader(SHADERDIR"CW/texturedVertex.glsl", SHADERDIR"CW/texturedFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"CW/skyboxVertex.glsl", SHADERDIR"CW/skyboxFragment.glsl");
	shadowShader = new Shader(SHADERDIR"CW/shadowVert.glsl", SHADERDIR"CW/shadowFrag.glsl");

	if (!solarShader->LinkProgram() ||
		!textShader->LinkProgram() ||
		!sunShader->LinkProgram() ||
		!skyboxShader->LinkProgram() ||
		!shadowShader->LinkProgram())
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

	//UpdateShaderMatrices();

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, (float*)&textureMatrix);

	//glActiveTexture(GL_TEXTURE0);

	DrawShadowScene();
	DrawCombinedScene();

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

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 6);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_CUBE_MAP, spaceMap);

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
}

void Renderer::DrawShadowScene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	SetCurrentShader(shadowShader);
	projMatrix = Matrix4::Perspective(1.f, 100000.f, 1.f, 90.f);

	viewMatrix.ToIdentity();// = Matrix4::BuildViewMatrix(sunLight->GetPosition(), Vector3(0, 0, 0));
	textureMatrix = biasMatrix * (projMatrix * viewMatrix);

	UpdateShaderMatrices();

	//DrawSkybox();
	//https://gamedev.stackexchange.com/questions/19461/opengl-glsl-render-to-cube-map

	//Matrix4 rotations[6] = 
	//{
	//	Matrix4::Rotation(-90.0f, Vector3(0, 1, 0)),	//West		-X
	//	Matrix4::Rotation(90.0f, Vector3(0, 1, 0)),		//East		+X
	//	Matrix4::Rotation(90.0f, Vector3(1, 0, 0)),		//Down		-Y
	//	Matrix4::Rotation(-90.0f, Vector3(1, 0, 0)),	//Up		+Y
	//	Matrix4::Rotation(0.0f, Vector3(0, 0, 0)),		//North		+Z
	//	Matrix4::Rotation(180.0f, Vector3(1, 0, 0))		//South		-Z
	//};

	Matrix4 rotations[6] =
	{
		Matrix4::BuildViewMatrix(Vector3(0,0,0), Vector3( 1, 0, 0), Vector3(0, -1, 0)),	//West		+X
		Matrix4::BuildViewMatrix(Vector3(0,0,0), Vector3(-1, 0, 0), Vector3(0,- 1, 0)),	//East		-X
		Matrix4::BuildViewMatrix(Vector3(0,0,0), Vector3(0, 1, 0), Vector3(-1, 0, 0)),	//Down		-Y
		Matrix4::BuildViewMatrix(Vector3(0,0,0), Vector3(0,  -1, 0), Vector3(-1, 0, 0)),	//Up	+Y
		Matrix4::BuildViewMatrix(Vector3(0,0,0), Vector3(0, 0, 1), Vector3(0, -1, 0)),	//North		+Z
		Matrix4::BuildViewMatrix(Vector3(0,0,0), Vector3(0, 0,  -1), Vector3(0, -1, 0))	//South		-Z
	};

	glEnable(GL_CULL_FACE);
	for (int face = 0; face < 6; face++)
	{
		Matrix4 temp = viewMatrix;
		viewMatrix = rotations[face];
		projMatrix = Matrix4::Perspective(1.f, 100000.f, 1.f, 90.f);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, shadowTex, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		DrawNode(ss);
		viewMatrix = temp;
	}

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);
	glDisable(GL_CULL_FACE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawCombinedScene()
{
	glDisable(GL_CULL_FACE);
	SetCurrentShader(solarShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 2);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

	SetShaderLight(*sunLight);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowTex);

	viewMatrix = camera->BuildViewMatrix();
	UpdateShaderMatrices();


	DrawSkybox();
	DrawNode(ss);

	glEnable(GL_CULL_FACE);
	glUseProgram(0);
}