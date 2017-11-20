#include "Renderer.h"

Renderer::Renderer(Window &parent) : OGLRenderer(parent)
{
	sunLight = new Light(Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 10000.0f);

	//Set up the sky map for each scene
	//skyMaps[SceneID::SOLAR_SCENE] = SOIL_load_OGL_cubemap(TEXTUREDIR"GalaxySkyBox/galaxy_west.bmp", TEXTUREDIR"GalaxySkyBox/galaxy_east.bmp", TEXTUREDIR"GalaxySkyBox/galaxy_up.bmp",
	//	TEXTUREDIR"GalaxySkyBox/galaxy_down.bmp", TEXTUREDIR"GalaxySkyBox/galaxy_south.bmp", TEXTUREDIR"GalaxySkyBox/galaxy_north.bmp",
	//	SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	//skyMaps[SceneID::SOLAR_SCENE] = SOIL_load_OGL_cubemap(TEXTUREDIR"HellSkyBox/hell_rt.bmp", TEXTUREDIR"HellSkyBox/hell_lf.bmp", TEXTUREDIR"HellSkyBox/hell_up.bmp",
	//	TEXTUREDIR"HellSkyBox/hell_dn.bmp", TEXTUREDIR"HellSkyBox/hell_bk.bmp", TEXTUREDIR"HellSkyBox/hell_ft.bmp",
	//	SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);

	skyMaps[SceneID::SOLAR_SCENE] = SOIL_load_OGL_cubemap(TEXTUREDIR"MercurySkyBox/mercury_rt.bmp", TEXTUREDIR"MercurySkyBox/mercury_lf.bmp", TEXTUREDIR"MercurySkyBox/mercury_up.bmp",
		TEXTUREDIR"MercurySkyBox/mercury_dn.bmp", TEXTUREDIR"MercurySkyBox/mercury_bk.bmp", TEXTUREDIR"MercurySkyBox/mercury_ft.bmp",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);

	skyMaps[SceneID::VOLCANO_SCENE] = SOIL_load_OGL_cubemap(TEXTUREDIR"HellSkyBox/hell_rt.bmp", TEXTUREDIR"HellSkyBox/hell_lf.bmp", TEXTUREDIR"HellSkyBox/hell_up.bmp",
		TEXTUREDIR"HellSkyBox/hell_dn.bmp", TEXTUREDIR"HellSkyBox/hell_bk.bmp", TEXTUREDIR"HellSkyBox/hell_ft.bmp",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);

	skyMaps[SceneID::MOUNTAIN_SCENE] = SOIL_load_OGL_cubemap(TEXTUREDIR"RustedSkyBox/rusted_west.jpg", TEXTUREDIR"RustedSkyBox/rusted_east.jpg", TEXTUREDIR"RustedSkyBox/rusted_up.jpg",
		TEXTUREDIR"RustedSkyBox/rusted_down.jpg", TEXTUREDIR"RustedSkyBox/rusted_south.jpg", TEXTUREDIR"RustedSkyBox/rusted_north.jpg",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);

	for (int i = 0; i < SceneID::NUM_SCENES; ++i)
	{
		if (!skyMaps[i])
		{
			return;
		}
	}

	currentSkyMap = skyMaps[sceneID];

	lavaQuad = Mesh::GenerateQuad();
	lavaQuad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"lavaPlanet2.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	lavaQuad->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"waterBumpMap1.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	volcanoHeightMap = new HeightMap(HEIGHTMAPSDIR"pompeii.data", 1080, 1080, 16.0f, 16.0f, 8.0f, 1.0f / 16.0f, 1.0f / 16.0f);
	volcanoHeightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	volcanoHeightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if (!lavaQuad->GetTexture() ||
		!lavaQuad->GetBumpMap() ||
		!volcanoHeightMap->GetTexture() ||
		!volcanoHeightMap->GetBumpMap())
	{
		return;
	}

	SetTextureRepeating(lavaQuad->GetTexture(), true);
	SetTextureRepeating(lavaQuad->GetBumpMap(), true);
	SetTextureRepeating(volcanoHeightMap->GetTexture(), true);
	SetTextureRepeating(volcanoHeightMap->GetBumpMap(), true);

	volcanoLight = new Light(Vector3((volcanoHeightMap->getRawHeight() * volcanoHeightMap->getHeightMapX() * 100.0f), 1000000.0f,
		volcanoHeightMap->getRawHeight() * volcanoHeightMap->getHeightMapX() * -60.0f), Vector4(1.0f, 0.7f, 0.4f, 1),
		volcanoHeightMap->getRawWidth() * volcanoHeightMap->getHeightMapX() * 100000.0f);

	waterQuad = Mesh::GenerateQuad();
	waterQuad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water2.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	waterQuad->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"waterBumpMap1.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	mountainsHeightMap = new HeightMap(HEIGHTMAPSDIR"hawaiiHeightMap.data", 1080, 1080, 16.0f, 16.0f, 16.0f, 1.0f / 16.0f, 1.0f / 16.0f);
	mountainsHeightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	mountainsHeightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	if (!waterQuad->GetTexture() || 
		!waterQuad->GetBumpMap() ||
		!mountainsHeightMap->GetTexture() ||
		!mountainsHeightMap->GetBumpMap())
	{
		return;
	}

	SetTextureRepeating(waterQuad->GetTexture(), true);
	SetTextureRepeating(waterQuad->GetBumpMap(), true);
	SetTextureRepeating(mountainsHeightMap->GetTexture(), true);
	SetTextureRepeating(mountainsHeightMap->GetBumpMap(), true);

	mountainsLight = new Light(Vector3((mountainsHeightMap->getRawHeight() * mountainsHeightMap->getHeightMapX() * 100.0f), 1000000.0f,
		mountainsHeightMap->getRawHeight() * mountainsHeightMap->getHeightMapX() * -60.0f), Vector4(1.0f, 0.7f, 0.4f, 1),
		mountainsHeightMap->getRawWidth() * mountainsHeightMap->getHeightMapX() * 100000.0f);

	cameras[SceneID::SOLAR_SCENE] = new Camera(-30.0f, 0.0f, Vector3(0, 1500.0f, 2500.0f));
	cameras[SceneID::VOLCANO_SCENE] = new Camera(0.0f, 0.0f, Vector3(volcanoHeightMap->getRawWidth() * volcanoHeightMap->getHeightMapX() / 2.0f, 5000.0f,
		volcanoHeightMap->getRawWidth() * volcanoHeightMap->getHeightMapX() / 2.0f));
	cameras[SceneID::MOUNTAIN_SCENE] = new Camera(0.0f, 0.0f, Vector3(mountainsHeightMap->getRawWidth() * mountainsHeightMap->getHeightMapX() / 2.0f, 5000.0f,
		mountainsHeightMap->getRawWidth() * mountainsHeightMap->getHeightMapX() / 2.0f));

	currentCamera = cameras[sceneID];

	waterRotate = 0.0f;

	currentShader = NULL;
	compileShaders();

	//Set up the shadow cube map for the solar system scene
	initShadowMap();

	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	defaultProjMatrix = Matrix4::Perspective(1.0f, 50000.0f, (float)width / (float)height, 45.0f);
	shadowMatrix = Matrix4::Perspective(1.0f, 10000.0f, 1.0f, 90.0f);
	projMatrix = defaultProjMatrix;

	ss = new SolarSystem();
	volcano = new Volcano();
	mountains = new Mountains();

	scenes[SceneID::SOLAR_SCENE] = ss;
	scenes[SceneID::VOLCANO_SCENE] = volcano;
	scenes[SceneID::MOUNTAIN_SCENE] = mountains;

	currentScene = scenes[sceneID];

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_DEPTH_TEST);
	init = true;
}

Renderer::~Renderer(void)
{
	currentScene = NULL;
	currentCamera = NULL;
	for (int i = 0; i < SceneID::NUM_SCENES; ++i)
	{
		delete cameras[i];
		delete scenes[i];
	}

	delete sunLight;
	delete basicFont;

	delete satelliteShader;
	delete sunShader;
	delete textShader;
	delete skyboxShader;
	delete blackHoleShader;
	currentShader = 0;

	delete mountainsLightShader;
	delete reflectShader;

	delete mountainsLight;
	delete mountainsHeightMap;

	delete volcanoHeightMap;
	delete volcanoLightShader;
	delete volcanoLight;

	delete waterQuad;
	delete lavaQuad;
}

void Renderer::UpdateScene(float msec)
{
	sceneTimer += msec;

	//Recompile shaders if R pressed
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_R))
	{
		compileShaders();
	}

	//Forward scene
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_RIGHT))
	{
		sceneTimer = 0.0f;
		sceneID++;
		if (sceneID >= SceneID::NUM_SCENES)
		{
			sceneID = 0;
		}

		changeScene();
	}

	//Backward scene
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_LEFT))
	{
		sceneTimer = 0.0f;
		sceneID--;
		if (sceneID < 0)
		{
			sceneID = SceneID::NUM_SCENES - 1;
		}

		changeScene();

	}

	currentCamera->UpdateCamera(msec);
	viewMatrix = currentCamera->BuildViewMatrix();

	currentScene->Update(msec);
	if (sceneID == SceneID::SOLAR_SCENE)
	{
	}	
	else if (sceneID == SceneID::MOUNTAIN_SCENE)
	{
		waterRotate += msec / 1000.0f;
	}
}

void Renderer::RenderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//glUseProgram(currentShader->GetProgram());

	if (sceneID == SceneID::SOLAR_SCENE)
	{
		SetShaderLight(*sunLight);

		//UpdateShaderMatrices();

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, (float*)&textureMatrix);
		float mod = 0.0f;
		glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "mod"), mod);

		//glActiveTexture(GL_TEXTURE0);

		DrawShadowScene();
		DrawCombinedScene();
	}
	else if (sceneID == SceneID::VOLCANO_SCENE)
	{
		DrawSkybox();
		DrawVolcanoMap();
		DrawVolcanoLava();
		DrawFloorLava();
	}
	else if (sceneID == SceneID::MOUNTAIN_SCENE)
	{
		DrawSkybox();
		DrawMountainMap();
		DrawWater();
	}

	//DrawInfo();

	SwapBuffers();
}

void Renderer::compileShaders()
{
	textShader = new Shader(SHADERDIR"Tutorials/texturedVertex.glsl", SHADERDIR"Tutorials/texturedFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"CW/skyboxVertex.glsl", SHADERDIR"CW/skyboxFragment.glsl");

	//Satellite shader (planets and moons) generates light and shadows
	satelliteShader = new Shader(SHADERDIR"CW/solarVertex.glsl", SHADERDIR"CW/solarFragment.glsl");
	//Sun uses a different shader as it doesn't need lighting
	sunShader = new Shader(SHADERDIR"CW/sunVertex.glsl", SHADERDIR"CW/sunFragment.glsl");
	//textShader = new Shader(SHADERDIR"CW/texturedVertex.glsl", SHADERDIR"CW/texturedFragment.glsl");
	shadowShader = new Shader(SHADERDIR"CW/shadowVert.glsl", SHADERDIR"CW/shadowFrag.glsl");
	blackHoleShader = new Shader(SHADERDIR"CW/sunVertex.glsl", SHADERDIR"CW/sunFragment.glsl",
		SHADERDIR"CW/blackHoleGeom.glsl", SHADERDIR"CW/tessControl.glsl", SHADERDIR"CW/tessEval.glsl");

	lavaShader = new Shader(SHADERDIR"CW/texturedVertex.glsl", SHADERDIR"CW/texturedFragment.glsl");
	volcanoLightShader = new Shader(SHADERDIR"CW/bumpVertex.glsl", SHADERDIR"CW/volcanoFragment.glsl");

	reflectShader = new Shader(SHADERDIR"CW/bumpVertex.glsl", SHADERDIR"CW/reflectFragment.glsl");
	mountainsLightShader = new Shader(SHADERDIR"CW/bumpVertex.glsl", SHADERDIR"CW/mountainFragment.glsl");

	if (!textShader->LinkProgram() ||
		!skyboxShader->LinkProgram() ||

		!satelliteShader->LinkProgram() ||
		!sunShader->LinkProgram() ||
		!shadowShader->LinkProgram() ||
		!blackHoleShader->LinkProgram() ||

		!lavaShader->LinkProgram() ||
		!volcanoLightShader->LinkProgram() ||

		!reflectShader->LinkProgram() ||
		!mountainsLightShader->LinkProgram())
	{
		return;
	}
	SetCurrentShader(satelliteShader);
}

void Renderer::initShadowMap()
{
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
}

void Renderer::changeScene()
{
	currentCamera = cameras[sceneID];
	currentScene = scenes[sceneID];
	currentSkyMap = skyMaps[sceneID];
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
		}
	}
	else if (n->getType() == RenderType::TYPE_SATELLITE)
	{
		SetCurrentShader(satelliteShader);
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
	glBindTexture(GL_TEXTURE_CUBE_MAP, currentSkyMap);

	UpdateShaderMatrices();
	waterQuad->Draw();

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

	//https://gamedev.stackexchange.com/questions/19461/opengl-glsl-render-to-cube-map

	glEnable(GL_CULL_FACE);
	for (int face = 0; face < 6; face++)
	{
		viewMatrix = shadowMapRotations[face];
		projMatrix = shadowMatrix;
		textureMatrix = biasMatrix * (projMatrix * viewMatrix);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, shadowTex, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		DrawNode(ss);
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
	SetCurrentShader(satelliteShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 2);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&currentCamera->GetPosition());

	SetShaderLight(*sunLight);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowTex);

	viewMatrix = currentCamera->BuildViewMatrix();
	UpdateShaderMatrices();

	DrawSkybox();
	DrawNode(ss);

	glEnable(GL_CULL_FACE);
	glUseProgram(0);
}

void Renderer::DrawVolcanoMap()
{
	SetCurrentShader(volcanoLightShader);
	SetShaderLight(*volcanoLight);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&currentCamera->GetPosition());

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	volcanoHeightMap->Draw();

	glUseProgram(0);
}

void Renderer::DrawMountainMap()
{
	SetCurrentShader(mountainsLightShader);
	SetShaderLight(*mountainsLight);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&currentCamera->GetPosition());

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	mountainsHeightMap->Draw();

	glUseProgram(0);
}

void Renderer::DrawWater()
{
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	SetCurrentShader(reflectShader);
	SetShaderLight(*mountainsLight);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&currentCamera->GetPosition());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mountainsHeightMap->GetBumpMap());

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, currentSkyMap);

	float heightX = (mountainsHeightMap->getRawWidth() * mountainsHeightMap->getHeightMapX() / 2.0f);
	float heightY = 256.0f * mountainsHeightMap->getHeightMapY() / 6.0f + sin(sceneTimer / 500.0f) * 20.0f;
	float heightZ = (mountainsHeightMap->getRawHeight() * mountainsHeightMap->getHeightMapZ() / 2.0f);

	modelMatrix =
		Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90.0f, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
		Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, -1.0f));

	UpdateShaderMatrices();
	waterQuad->Draw();

	glUseProgram(0);
	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void Renderer::DrawFloorLava()
{
	SetCurrentShader(lavaShader);
	SetShaderLight(*volcanoLight);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&currentCamera->GetPosition());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mountainsHeightMap->GetBumpMap());

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, currentSkyMap);

	float heightX = (volcanoHeightMap->getRawWidth() * volcanoHeightMap->getHeightMapX() / 2.0f);
	float heightY = 256.0f * volcanoHeightMap->getHeightMapY() / 6.0f + sin(sceneTimer / 500.0f) * 20.0f;
	float heightZ = (volcanoHeightMap->getRawHeight() * volcanoHeightMap->getHeightMapZ() / 2.0f);

	modelMatrix =
		Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
		Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();
	lavaQuad->Draw();

	glUseProgram(0);
}

void Renderer::DrawVolcanoLava()
{
	SetCurrentShader(lavaShader);
	SetShaderLight(*volcanoLight);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&currentCamera->GetPosition());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mountainsHeightMap->GetBumpMap());

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, currentSkyMap);

	float heightX = (volcanoHeightMap->getRawWidth() * volcanoHeightMap->getHeightMapX() / 80.0f);
	float heightY = 256.0f * volcanoHeightMap->getHeightMapY() * 0.85f + sin(sceneTimer / 800.0f) * 8.0f;
	float heightZ = (volcanoHeightMap->getRawHeight() * volcanoHeightMap->getHeightMapZ() / 80.0f);

	modelMatrix =
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Translation(Vector3(28.5f, heightY, 46.66f)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(1.0f, 1.0f, 1.0f)) *
		Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();
	lavaQuad->Draw();

	glUseProgram(0);
}

void Renderer::DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective) {
	//Create a new temporary TextMesh, using our line of text and our font
	TextMesh* mesh = new TextMesh(text, *basicFont);

	//This just does simple matrix setup to render in either perspective or
	//orthographic mode, there's nothing here that's particularly tricky.
	if (perspective) {
		modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size, size, 1));
		viewMatrix = currentCamera->BuildViewMatrix();
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