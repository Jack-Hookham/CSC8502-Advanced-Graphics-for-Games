#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)
{
	mod = 0.0f;
	sunLight = new Light(Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 10000.0f);
	//SolarSystem::createSphereObj();
	camera = new Camera();

	quad = Mesh::GenerateQuad();
	spaceMap = SOIL_load_OGL_cubemap(TEXTUREDIR"galaxy_west.bmp", TEXTUREDIR"galaxy_east.bmp", TEXTUREDIR"galaxy_up.bmp",
		TEXTUREDIR"galaxy_down.bmp", TEXTUREDIR"galaxy_south.bmp", TEXTUREDIR"galaxy_north.bmp",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

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

	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glGenFramebuffers(1, &shadowFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	SetCurrentShader(solarShader);

	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	defaultProjMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	shadowMatrix = Matrix4::Perspective(1.0f, 10000.0f, 1.0f, 90.0f);
	projMatrix = defaultProjMatrix;

	camera->SetPosition(Vector3(0, 30.0f, 500.0f));

	ss = new SolarSystem();

	root = new SceneNode();
	root->AddChild((SceneNode*)ss);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_DEPTH_TEST);
	init = true;
}

Renderer::~Renderer(void)
{
	delete root;

	delete camera;
	delete sunLight;
	delete basicFont;

	delete solarShader;
	delete sunShader;
	delete textShader;
	delete skyboxShader;
	delete blackHoleShader;
	currentShader = NULL;

	delete quad;

	//SolarSystem::deleteSphereObj();
}

void Renderer::UpdateScene(float msec)
{
	//mod += msec * 0.4f;

	//Recompile shaders if R pressed
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_R))
	{
		compileShaders();
	}

	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	root->Update(msec);
}

void Renderer::compileShaders()
{
	solarShader = new Shader(SHADERDIR"CW/solarVertex.glsl", SHADERDIR"CW/solarFragment.glsl");
	//Sun uses a different shader as it doesn't need lighting
	sunShader = new Shader(SHADERDIR"CW/sunVertex.glsl", SHADERDIR"CW/sunFragment.glsl");
	//textShader = new Shader(SHADERDIR"CW/texturedVertex.glsl", SHADERDIR"CW/texturedFragment.glsl");
	textShader = new Shader(SHADERDIR"Tutorials/texturedVertex.glsl", SHADERDIR"Tutorials/texturedFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"CW/skyboxVertex.glsl", SHADERDIR"CW/skyboxFragment.glsl");
	shadowShader = new Shader(SHADERDIR"CW/shadowVert.glsl", SHADERDIR"CW/shadowFrag.glsl");	
	blackHoleShader = new Shader(SHADERDIR"CW/sunVertex.glsl", SHADERDIR"CW/sunFragment.glsl",	
		SHADERDIR"CW/blackHoleGeom.glsl", SHADERDIR"CW/tessControl.glsl", SHADERDIR"CW/tessEval.glsl");

	//blackHoleShader = new Shader(SHADERDIR"CW/tessVert.glsl", SHADERDIR"CW/basicFrag.glsl",
	//	SHADERDIR"CW/blackHoleGeom.glsl", SHADERDIR"CW/tessControl.glsl", SHADERDIR"CW/tessEval.glsl");

	if (!solarShader->LinkProgram() ||
		!textShader->LinkProgram() ||
		!sunShader->LinkProgram() ||
		!skyboxShader->LinkProgram() ||
		!shadowShader->LinkProgram() ||
		!blackHoleShader->LinkProgram())
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
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "mod"), mod);

	//glActiveTexture(GL_TEXTURE0);

	DrawShadowScene();
	DrawCombinedScene();

	DrawInfo();

	SwapBuffers();
}

void Renderer::DrawInfo()
{
	glEnable(GL_BLEND);

	//glUseProgram(textShader->GetProgram());
	SetCurrentShader(textShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), basicFont->texture);

	DrawText("This is orthographic text!", Vector3(0, 0, 0), 16.0f);
	DrawText("This is perspective text!!!!", Vector3(0, 0, -1000), 64.0f, true);

	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2) << fps;
	DrawText(oss.str(), Vector3(50.0f, 50.0f, 0), 16.0f);

	glUseProgram(0);
	glDisable(GL_BLEND);
}

void Renderer::DrawNode(RenderObject* n)
{
	if (n->getType() == RenderType::TYPE_SUN)
	{
		if (ss->getExploding())
		{
			SetCurrentShader(blackHoleShader);
		}
		else
		{
			SetCurrentShader(sunShader);
			//SetCurrentShader(solarShader);
		}
	}
	else if (n->getType() == RenderType::TYPE_PLANET || n->getType() == RenderType::TYPE_MOON)
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

	for (vector<RenderObject*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i)
	{
		DrawNode(*i);
	}
}

void Renderer::DrawSkybox()
{
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 3);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, spaceMap);

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDepthMask(GL_TRUE);
}

void Renderer::DrawShadowScene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	SetCurrentShader(shadowShader);
	//projMatrix = Matrix4::Perspective(1.0f, 100000.0f, 1.0f, 90.0f);

	viewMatrix= Matrix4::BuildViewMatrix(sunLight->GetPosition(), Vector3(0, 0, 0));
	textureMatrix = biasMatrix * (projMatrix * viewMatrix);

	UpdateShaderMatrices();

	//DrawSkybox();
	//https://gamedev.stackexchange.com/questions/19461/opengl-glsl-render-to-cube-map

	glEnable(GL_CULL_FACE);
	for (int face = 0; face < 6; face++)
	{
		Matrix4 temp = viewMatrix;
		viewMatrix = shadowMapRotations[face];
		projMatrix = shadowMatrix;
		textureMatrix = biasMatrix * (projMatrix * viewMatrix);
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
	projMatrix = defaultProjMatrix;
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

void Renderer::DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective) {
	//Create a new temporary TextMesh, using our line of text and our font
	TextMesh* mesh = new TextMesh(text, *basicFont);

	//This just does simple matrix setup to render in either perspective or
	//orthographic mode, there's nothing here that's particularly tricky.
	if (perspective) {
		modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	}
	else {
		//In ortho mode, we subtract the y from the height, so that a height of 0
		//is at the top left of the screen, which is more intuitive
		//(for me anyway...)
		modelMatrix = Matrix4::Translation(Vector3(position.x, height - position.y, position.z)) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix.ToIdentity();
		projMatrix = Matrix4::Orthographic(-1.0f, 1.0f, (float)width, 0.0f, (float)height, 0.0f);
	}
	//Either way, we update the matrices, and draw the mesh
	UpdateShaderMatrices();
	mesh->Draw();

	delete mesh; //Once it's drawn, we don't need it anymore!
}