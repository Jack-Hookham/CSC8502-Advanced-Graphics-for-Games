#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)
{
	sunLight = new Light(Vector3(0.0f, 2000.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1000000.0f);
	//SolarSystem::createSphereObj();
	camera = new Camera();

	currentShader = new Shader(SHADERDIR"CW/solarVertex.glsl", SHADERDIR"CW/solarFragment.glsl");
	ringShader = new Shader(SHADERDIR"CW/ringVertex.glsl", SHADERDIR"CW/ringFragment.glsl");
	textShader = new Shader(SHADERDIR"CW/texturedVertex.glsl", SHADERDIR"CW/texturedFragment.glsl");

	if (!currentShader->LinkProgram() || !ringShader->LinkProgram() || !textShader->LinkProgram())
	{
		return;
	}

	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	camera->SetPosition(Vector3(0, 30.0f, 500.0f));

	root = new SceneNode();	
	SolarSystem* ss = new SolarSystem();

	ss->getPlanet()->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	ss->getPlanet2()->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"saturnmap.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	ss->getSun()->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"sunmap.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	ss->getMoon()->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	//ss->getMoon()->GetMesh()->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"sunmap.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	if (!ss->getMoon()->GetMesh()->GetTexture())
	{
		return;
	}

	SetTextureRepeating(ss->getPlanet()->GetMesh()->GetTexture(), true);
	SetTextureRepeating(ss->getPlanet2()->GetMesh()->GetTexture(), true);
	SetTextureRepeating(ss->getMoon()->GetMesh()->GetTexture(), true);
	SetTextureRepeating(ss->getSun()->GetMesh()->GetTexture(), true);

	root->AddChild(ss);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_DEPTH_TEST);
	init = true;
}

Renderer::~Renderer(void)
{
	delete root;
	delete camera;
	delete sunLight;
	delete basicFont;

	delete ringShader;
	delete textShader;

	//SolarSystem::deleteSphereObj();
}

void Renderer::UpdateScene(float msec)
{
	//Recompile shaders if R pressed
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_R))
	{
		currentShader = new Shader(SHADERDIR"CW/solarVertex.glsl", SHADERDIR"CW/solarFragment.glsl");

		if (!currentShader->LinkProgram())
		{
			return;
		}
	}

	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	root->Update(msec);
}

void Renderer::RenderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glUseProgram(currentShader->GetProgram());

	SetShaderLight(*sunLight);

	UpdateShaderMatrices();

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, (float*)&textureMatrix);

	//glActiveTexture(GL_TEXTURE0);

	DrawNode(root);
	drawText();

	glUseProgram(0);
	SwapBuffers();
}

void Renderer::drawText()
{
	glEnable(GL_BLEND);

	glUseProgram(textShader->GetProgram());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), basicFont->texture);

	//DrawText("This is orthographic text!", Vector3(0, 0, 0), 16.0f);

	//std::ostringstream oss;
	//oss << std::fixed << std::setprecision(2) << fps;
	//DrawText(oss.str(), Vector3(50.0f, 50.0f, 0), 16.0f);

	glDisable(GL_BLEND);
}

void Renderer::DrawNode(SceneNode* n)
{
	if (n->GetMesh())
	{
		Matrix4 transform = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)&transform);
		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)n->GetMesh()->GetTexture());
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

		n->Draw();
	}

	for (vector<SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i)
	{
		DrawNode(*i);
	}
}