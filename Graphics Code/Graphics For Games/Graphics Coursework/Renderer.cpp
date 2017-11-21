#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent)
{
	//Post processing setup
	processQuad = Mesh::GenerateQuad();

	//Generate scene depth texture
	glGenTextures(1, &bufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	//Colour texture
	for (int i = 0; i < 2; ++i)
	{
		glGenTextures(1, &bufferColourTex[i]);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	glGenFramebuffers(1, &bufferFBO);		//Render the scene into this
	glGenFramebuffers(1, &processFBO);		//And do post processing in this

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);

	//Check FBO attachment success
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !bufferDepthTex || !bufferColourTex[0])
	{
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	sunLight = new Light(Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 10000.0f);

	//Set up the sky map for each scene
	skyMaps[SceneID::SOLAR_SCENE] = SOIL_load_OGL_cubemap(TEXTUREDIR"GalaxySkyBox/galaxy_west.bmp", TEXTUREDIR"GalaxySkyBox/galaxy_east.bmp", TEXTUREDIR"GalaxySkyBox/galaxy_up.bmp",
		TEXTUREDIR"GalaxySkyBox/galaxy_down.bmp", TEXTUREDIR"GalaxySkyBox/galaxy_south.bmp", TEXTUREDIR"GalaxySkyBox/galaxy_north.bmp",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	//skyMaps[SceneID::SOLAR_SCENE] = SOIL_load_OGL_cubemap(TEXTUREDIR"HellSkyBox/hell_rt.bmp", TEXTUREDIR"HellSkyBox/hell_lf.bmp", TEXTUREDIR"HellSkyBox/hell_up.bmp",
	//	TEXTUREDIR"HellSkyBox/hell_dn.bmp", TEXTUREDIR"HellSkyBox/hell_bk.bmp", TEXTUREDIR"HellSkyBox/hell_ft.bmp",
	//	SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);

	//skyMaps[SceneID::SOLAR_SCENE] = SOIL_load_OGL_cubemap(TEXTUREDIR"MercurySkyBox/mercury_rt.bmp", TEXTUREDIR"MercurySkyBox/mercury_lf.bmp", TEXTUREDIR"MercurySkyBox/mercury_up.bmp",
	//	TEXTUREDIR"MercurySkyBox/mercury_dn.bmp", TEXTUREDIR"MercurySkyBox/mercury_bk.bmp", TEXTUREDIR"MercurySkyBox/mercury_ft.bmp",
	//	SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);

	skyMaps[SceneID::VOLCANO_SCENE] = SOIL_load_OGL_cubemap(TEXTUREDIR"HellSkyBox/hell_rt.bmp", TEXTUREDIR"HellSkyBox/hell_lf.bmp", TEXTUREDIR"HellSkyBox/hell_up.bmp",
		TEXTUREDIR"HellSkyBox/hell_dn.bmp", TEXTUREDIR"HellSkyBox/hell_bk.bmp", TEXTUREDIR"HellSkyBox/hell_ft.bmp",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);

	//skyMaps[SceneID::MOUNTAIN_SCENE] = SOIL_load_OGL_cubemap(TEXTUREDIR"RustedSkyBox/rusted_west.jpg", TEXTUREDIR"RustedSkyBox/rusted_east.jpg", TEXTUREDIR"RustedSkyBox/rusted_up.jpg",
	//	TEXTUREDIR"RustedSkyBox/rusted_down.jpg", TEXTUREDIR"RustedSkyBox/rusted_south.jpg", TEXTUREDIR"RustedSkyBox/rusted_north.jpg",
	//	SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);

	skyMaps[SceneID::MOUNTAIN_SCENE] = SOIL_load_OGL_cubemap(TEXTUREDIR"sb_frozen/frozen_rt.bmp", TEXTUREDIR"sb_frozen/frozen_lf.bmp", TEXTUREDIR"sb_frozen/frozen_up.bmp",
		TEXTUREDIR"sb_frozen/frozen_dn.bmp", TEXTUREDIR"sb_frozen/frozen_bk.bmp", TEXTUREDIR"sb_frozen/frozen_ft.bmp",
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

	volcanoHeightMap = new HeightMap(HEIGHTMAPSDIR"pompeii257.data", 257, 257, 16.0f, 16.0f, 8.0f, 1.0f / 16.0f, 1.0f / 16.0f);
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

	lavaEmitter = new ParticleEmitter(ParticleType::LAVA_PARTICLE);
	lavaEmitter->SetParticleSize(20.0f);
	lavaEmitter->SetParticleVariance(1.0f);
	lavaEmitter->SetLaunchParticles(32.0f);
	lavaEmitter->SetParticleLifetime(3000.0f);
	lavaEmitter->SetParticleSpeed(1.0f);

	emberEmitter = new ParticleEmitter(ParticleType::EMBER_PARTICLE);
	emberEmitter->SetParticleSize(10.0f);
	emberEmitter->SetParticleVariance(1.0f);
	emberEmitter->SetLaunchParticles(16.0f);
	emberEmitter->SetParticleLifetime(3000.0f);
	emberEmitter->SetParticleSpeed(1.0f);

	steamEmitter = new ParticleEmitter(ParticleType::STEAM_PARTICLE);
	steamEmitter->SetParticleSize(10.0f);
	steamEmitter->SetParticleVariance(1.0f);
	steamEmitter->SetLaunchParticles(16.0f);
	steamEmitter->SetParticleLifetime(3000.0f);
	steamEmitter->SetParticleSpeed(0.1f);

	volcanoLight = new Light(Vector3((volcanoHeightMap->getRawHeight() * volcanoHeightMap->getHeightMapX() * 0.25f), 3000.0f,
		volcanoHeightMap->getRawHeight() * volcanoHeightMap->getHeightMapX() * 0.75f), Vector4(1.0f, 1.0f, 1.0f, 1), 10000.0f);

	waterQuad = Mesh::GenerateQuad();
	waterQuad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water2.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	waterQuad->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"waterBumpMap1.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	mountainsHeightMap = new HeightMap(HEIGHTMAPSDIR"mountainsTest2.data", 257, 257, 16.0f, 16.0f, 4.0f, 1.0f / 16.0f, 1.0f / 16.0f);
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

	minSunX = -15000.0f + mountainsHeightMap->getRawHeight() * mountainsHeightMap->getHeightMapX() / 2.0f;
	maxSunX = 15000.0f + mountainsHeightMap->getRawHeight() * mountainsHeightMap->getHeightMapX();
	mountainsLightStart = Vector3(minSunX, -1000.0f,
		mountainsHeightMap->getRawHeight() * mountainsHeightMap->getHeightMapX() / 2.0f);

	mountainsLight = new Light(mountainsLightStart, Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		10000.0f);

	cameras[SceneID::SOLAR_SCENE] = new Camera(-30.0f, 0.0f, Vector3(0, 1500.0f, 2500.0f));
	cameras[SceneID::VOLCANO_SCENE] = new Camera(15.0f, 160.0f, Vector3(mountainsHeightMap->getRawWidth() * mountainsHeightMap->getHeightMapX() * 0.9f, 800.0f,
		mountainsHeightMap->getRawWidth() * mountainsHeightMap->getHeightMapX() * -0.5f));
	cameras[SceneID::MOUNTAIN_SCENE] = new Camera(0.0f, 0.0f, Vector3(mountainsHeightMap->getRawWidth() * mountainsHeightMap->getHeightMapX() / 2.0f, 1000.0f,
		mountainsHeightMap->getRawWidth() * mountainsHeightMap->getHeightMapX() / 2.0f));

	currentCamera = cameras[sceneID];

	currentShader = NULL;
	compileShaders();

	//Set up the shadow cube map for the solar system scene
	initShadowMap();

	basicFont = new Font(SOIL_load_OGL_texture(TEXTUREDIR"tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT), 16, 16);

	defaultProjMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	shadowMatrix = Matrix4::Perspective(1.0f, 10000.0f, 1.0f, 90.0f);
	projMatrix = defaultProjMatrix;

	ss = new SolarSystem();
	volcano = new Volcano();
	mountains = new Mountains();

	scenes[SceneID::SOLAR_SCENE] = ss;
	scenes[SceneID::VOLCANO_SCENE] = volcano;
	scenes[SceneID::MOUNTAIN_SCENE] = mountains;

	currentScene = scenes[sceneID];

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	init = true;
}

Renderer::~Renderer(void)
{
	delete processQuad;
	delete processShader;
	delete sceneShader;

	glDeleteTextures(2, bufferColourTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &processFBO);

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
	delete mountainSkyboxShader;

	delete volcanoHeightMap;
	delete volcanoLightShader;
	delete volcanoLight;
	delete particleShader;

	delete waterQuad;
	delete lavaQuad;

	delete lavaEmitter;
	delete emberEmitter;
	delete steamEmitter;
}

void Renderer::UpdateScene(float msec)
{
	sceneTimer += msec;
	totalTimer += msec;

	//Calculate average fps every second
	//if over 1 second has passed
	fpsTimer += msec;
	if (fpsTimer > 1000.0f)
	{
		fps = frameCount * 1000.0f / fpsTimer;
		frameCount = 0;
		fpsTimer = 0.0f;
	}
	frameCount++;

	//Recompile shaders if R pressed
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_R))
	{
		compileShaders();
	}

	//Forward scene
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_RIGHT))
	{
		sceneID++;
		if (sceneID >= SceneID::NUM_SCENES)
		{
			sceneID = 0;
		}

		//Change current scene information
		setScene(sceneID);
	}

	//Backward scene
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_LEFT))
	{
		sceneID--;
		if (sceneID < 0)
		{
			sceneID = SceneID::NUM_SCENES - 1;
		}

		//Change current scene information
		setScene(sceneID);
	}

	//Toggle info
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_I))
	{
		showInfo = !showInfo;
	}

	currentCamera->UpdateCamera(msec);
	viewMatrix = currentCamera->BuildViewMatrix();

	currentScene->Update(msec);
	if (sceneID == SceneID::SOLAR_SCENE)
	{

	}	
	else if (sceneID == SceneID::VOLCANO_SCENE)
	{
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_E))
		{
			volcanoErupting = !volcanoErupting;
		}
		lavaEmitter->Update(msec, volcanoErupting);
		emberEmitter->Update(msec);
		//steamEmitter->Update(msec);

		if (volcanoErupting)
		{
			shakeCamera(msec, currentCamera);
		}
	}
	else if (sceneID == SceneID::MOUNTAIN_SCENE)
	{
		//Move sun light to simulate day/night cycle
		int yDir;		//Determine whether the sun is going up or down and store it here
		//Rise until over centre
		float middleX = mountainsHeightMap->getRawHeight() * mountainsHeightMap->getHeightMapX() / 2.0f;
		if (mountainsLight->GetPosition().x < middleX)
		{
			yDir = 1;
		}
		else
		{
			yDir = -1;
		}

		mountainsLight->SetPosition(mountainsLight->GetPosition() + Vector3(1.0f * msec, 0.3f * msec * yDir, 0.0f));

		if (mountainsLight->GetPosition().x > maxSunX)
		{
			mountainsLight->SetPosition(mountainsLightStart);
		}

		//3400 is roughly the hightest the sun gets
		//sunStrength is a multiplier for the skybox colour
		sunStrength = mountainsLight->GetPosition().y / 3400.0f;
	}
}

void Renderer::RenderScene()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//glUseProgram(currentShader->GetProgram());

	if (sceneID == SceneID::SOLAR_SCENE)
	{
		SetShaderLight(*sunLight);

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
		DrawEmitters();
	}
	else if (sceneID == SceneID::MOUNTAIN_SCENE)
	{
		DrawSkybox();
		DrawMountainMap();
		DrawWater();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//DrawPostProcess();
	//DrawFinalScene();

	if (showInfo)
	{
		drawInfo();
	}

	SwapBuffers();
}

void Renderer::setScene(int n)
{
	sceneTimer = 0.0f;
	sceneID = n;
	currentScene = scenes[sceneID];
	currentCamera = cameras[sceneID];
	currentSkyMap = skyMaps[sceneID];
}

void Renderer::compileShaders()
{
	processShader = new Shader(SHADERDIR"Tutorials/texturedVertex.glsl", SHADERDIR"Tutorials/processFragment.glsl");
	sceneShader = new Shader(SHADERDIR"Tutorials/texturedVertex.glsl", SHADERDIR"Tutorials/texturedFragment.glsl");
	textShader = new Shader(SHADERDIR"CW/texturedVertex.glsl", SHADERDIR"CW/texturedFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"CW/skyboxVertex.glsl", SHADERDIR"CW/skyboxFragment.glsl");

	//Satellite shader (planets and moons) generates light and shadows
	satelliteShader = new Shader(SHADERDIR"CW/solarVertex.glsl", SHADERDIR"CW/solarFragment.glsl");
	//Sun uses a different shader as it doesn't need lighting
	sunShader = new Shader(SHADERDIR"CW/sunVertex.glsl", SHADERDIR"CW/sunFragment.glsl");
	//textShader = new Shader(SHADERDIR"CW/texturedVertex.glsl", SHADERDIR"CW/texturedFragment.glsl");
	shadowShader = new Shader(SHADERDIR"CW/shadowVert.glsl", SHADERDIR"CW/shadowFrag.glsl");
	blackHoleShader = new Shader(SHADERDIR"CW/sunVertex.glsl", SHADERDIR"CW/sunFragment.glsl",
		SHADERDIR"CW/blackHoleGeom.glsl", SHADERDIR"CW/tessControl.glsl", SHADERDIR"CW/tessEval.glsl");

	lavaShader = new Shader(SHADERDIR"CW/lavaVertex.glsl", SHADERDIR"CW/lavaFragment.glsl");
	volcanoLightShader = new Shader(SHADERDIR"CW/bumpVertex.glsl", SHADERDIR"CW/volcanoFragment.glsl");
	particleShader = new Shader(SHADERDIR"CW/particleVertex.glsl", SHADERDIR"CW/particleFragment.glsl", SHADERDIR"CW/particleGeometry.glsl");

	reflectShader = new Shader(SHADERDIR"CW/bumpVertex.glsl", SHADERDIR"CW/reflectFragment.glsl");
	mountainsLightShader = new Shader(SHADERDIR"CW/bumpVertex.glsl", SHADERDIR"CW/mountainFragment.glsl");
	mountainSkyboxShader = new Shader(SHADERDIR"CW/cycleSkyboxVertex.glsl", SHADERDIR"CW/cycleSkyboxFragment.glsl");

	if (!processShader->LinkProgram() ||
		!sceneShader->LinkProgram() ||
		!textShader->LinkProgram() ||
		!skyboxShader->LinkProgram() ||

		!satelliteShader->LinkProgram() ||
		!sunShader->LinkProgram() ||
		!shadowShader->LinkProgram() ||
		!blackHoleShader->LinkProgram() ||

		!lavaShader->LinkProgram() ||
		!volcanoLightShader->LinkProgram() ||
		!particleShader->LinkProgram() ||

		!reflectShader->LinkProgram() ||
		!mountainsLightShader->LinkProgram() ||
		!mountainSkyboxShader->LinkProgram())
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

void Renderer::drawInfo()
{
	glEnable(GL_BLEND);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	SetCurrentShader(textShader);

	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	float currentY = 0.0f;

	//Current scene
	std::ostringstream oss;
	oss << "Current Scene: " << std::fixed << std::setprecision(0) << sceneNames[sceneID];
	drawText(oss.str(), Vector3(0.0f, currentY, 0.0f), 16.0f);
	currentY += 20.0f;

	//FPS
	oss.str("");
	oss.clear();
	oss << "FPS: " << std::fixed << std::setprecision(2) << fps;
	drawText(oss.str(), Vector3(0.0f, currentY, 0.0f), 16.0f);
	currentY += 20.0f;

	//Camera speed
	oss.str("");
	oss.clear();
	oss << "Camera Speed: " << std::fixed << std::setprecision(1) << currentCamera->getSpeed();
	drawText(oss.str(), Vector3(0.0f, currentY, 0.0f), 16.0f);
	currentY += 20.0f;

	//Current position
	oss.str("");
	oss.clear();
	oss << "Camera Position: " << std::fixed << std::setprecision(0) << currentCamera->GetPosition();
	drawText(oss.str(), Vector3(0.0f, currentY, 0.0f), 16.0f);
	currentY += 20.0f;

	//Total time
	oss.str("");
	oss.clear();
	oss << "Total Time: " << std::fixed << std::setprecision(2) << totalTimer / 1000.0f;
	drawText(oss.str(), Vector3(0.0f, currentY, 0.0f), 16.0f);
	currentY += 20.0f;

	//Scene time
	oss.str("");
	oss.clear();
	oss << "Scene Time: " << std::fixed << std::setprecision(2) << sceneTimer / 1000.0f;
	drawText(oss.str(), Vector3(0.0f, currentY, 0.0f), 16.0f);
	currentY += 20.0f;

	glUseProgram(0);

	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
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

void Renderer::DrawPostProcess()
{
	glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	SetCurrentShader(processShader);

	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	UpdateShaderMatrices();

	glDisable(GL_DEPTH_TEST);

	glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);

	for (int i = 0; i < POST_PASSES; ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 0);

		processQuad->SetTexture(bufferColourTex[0]);
		processQuad->Draw();

		//Now swap the colour buffers and do the second blur pass
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);

		processQuad->SetTexture(bufferColourTex[1]);
		processQuad->Draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);

	glEnable(GL_DEPTH_TEST);
}
void Renderer::DrawFinalScene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	SetCurrentShader(sceneShader);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	UpdateShaderMatrices();
	processQuad->SetTexture(bufferColourTex[0]);
	processQuad->Draw();
	glUseProgram(0);
}

void Renderer::shakeCamera(const float msec, Camera* camera)
{
	float min = 0.5f * msec;
	//Camera shake
	Vector3 oldPosition = currentCamera->GetPosition();
	currentCamera->SetPosition(Vector3(oldPosition.x + (RAND() * min * 2.0f) - min, oldPosition.y + (RAND() * min * 2.0f) - min,
		oldPosition.z + (RAND() * min * 2.0f) - min));
}

void Renderer::DrawSkybox()
{
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDepthMask(GL_FALSE);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	if (sceneID == SceneID::MOUNTAIN_SCENE)
	{
		SetCurrentShader(mountainSkyboxShader);
		glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "sunStrength"), sunStrength);
	}
	else
	{
		SetCurrentShader(skyboxShader);
	}

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 3);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, currentSkyMap);

	UpdateShaderMatrices();
	waterQuad->Draw();

	glUseProgram(0);
	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDepthMask(GL_TRUE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
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
	glEnable(GL_CULL_FACE);
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
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	float heightY = 256.0f * mountainsHeightMap->getHeightMapY() / 3.0f + sin(sceneTimer / 500.0f) * 10.0f;
	float heightZ = (mountainsHeightMap->getRawHeight() * mountainsHeightMap->getHeightMapZ() / 2.0f);

	modelMatrix =
		Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90.0f, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f))
		* Matrix4::Translation(Vector3(0.0f, -sin(sceneTimer / 500.0f) / 100.0f, 0.0f));

	UpdateShaderMatrices();
	waterQuad->Draw();

	glUseProgram(0);

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void Renderer::DrawFloorLava()
{
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
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
		Matrix4::Scale(Vector3(heightX * 3.0f, 1, heightZ * 3.0f)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f));

	UpdateShaderMatrices();
	lavaQuad->Draw();

	glUseProgram(0);
	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void Renderer::DrawVolcanoLava()
{
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
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

	float heightX = (volcanoHeightMap->getRawWidth() * volcanoHeightMap->getHeightMapX() / 40.0f);
	float heightY = 256.0f * volcanoHeightMap->getHeightMapY() * 0.73f + sin(sceneTimer / 800.0f) * 8.0f;
	float heightZ = (volcanoHeightMap->getRawHeight() * volcanoHeightMap->getHeightMapZ() / 40.0f);

	modelMatrix =
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Translation(Vector3(21.5f, heightY, 20.2f)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(1.0f, 1.0f, 1.0f));

	UpdateShaderMatrices();
	lavaQuad->Draw();

	glUseProgram(0);
	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void Renderer::DrawEmitters()
{
	//glClearColor(0, 0, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	modelMatrix.ToIdentity();
	projMatrix = defaultProjMatrix;

	SetCurrentShader(particleShader);
	//SetShaderLight(*volcanoLight);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	SetShaderParticleSize(lavaEmitter->GetParticleSize());
	UpdateShaderMatrices();
	lavaEmitter->Draw();

	emberEmitter->SetParticleSize(10.0f + RAND() * 10.0f);
	SetShaderParticleSize(emberEmitter->GetParticleSize());
	UpdateShaderMatrices();
	emberEmitter->Draw();

	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
}

void Renderer::SetShaderParticleSize(const float f)
{
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "particleSize"), f);
}

void Renderer::drawText(const std::string &text, const Vector3 &position, const float size, const bool perspective) {
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

	projMatrix = defaultProjMatrix;
}