#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{
	camera = new Camera(-8.0f, 40.0f, Vector3(-200.0f, 50.0f, 250.0f));
	light = new Light(Vector3(-450.0f, 200.0f, 280.0f), Vector4(1, 1, 1, 1), 5500.0f);

	hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	hellNode = new MD5Node(*hellData);

	hellData->AddAnim(MESHDIR"idle2.md5anim");
	hellNode->PlayAnim(MESHDIR"idle2.md5anim");

	sceneShader = new Shader(SHADERDIR"shadowSceneVert.glsl", SHADERDIR"shadowSceneFrag.glsl");
}

Renderer::~Renderer(void)
{
}

void Renderer::UpdateScene(float msec)
{
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
}

void Renderer::DrawMesh()
{
}

void Renderer::DrawFloor()
{
}

void Renderer::DrawShadowScene()
{
}

void Renderer::DrawCombinedScene()
{
}