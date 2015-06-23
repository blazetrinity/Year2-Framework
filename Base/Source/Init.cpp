#include "GenerateSkyPlane.h"

void GenerateSkyPlane::InitObjects()
{
	for(int i = 0; i < CObjectClass::NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[CObjectClass::GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[CObjectClass::GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[CObjectClass::GEO_QUAD]->textureID[0] = LoadTGA("Image//calibri.tga");
	meshList[CObjectClass::GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[CObjectClass::GEO_TEXT]->textureID[0] = LoadTGA("Image//calibri.tga");
	meshList[CObjectClass::GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[CObjectClass::GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);

	meshList[CObjectClass::GEO_POND] = MeshBuilder::GenerateQuad("GEO_POND", Color(1, 1, 1), 1.f);
	meshList[CObjectClass::GEO_POND]->textureID[0] = LoadTGA("Image//sea.tga");

	meshList[CObjectClass::GEO_SKYPLANE] = MeshBuilder::GenerateSkyPlane("GEO_SKYPLANE", Color(1,1,1), 128, 1000.0f, 2500.0f, 1.0f, 1.0f);
	meshList[CObjectClass::GEO_SKYPLANE]->textureID[0] = LoadTGA("Image//Sky.tga");

	meshList[CObjectClass::GEO_TERRAIN] = MeshBuilder::GenerateTerrain("GEO_TERRAIN", "Image//heightmap.raw", m_heightMap);
	meshList[CObjectClass::GEO_TERRAIN]->textureID[0] = LoadTGA("Image//Grass.tga");
	meshList[CObjectClass::GEO_TERRAIN]->textureID[1] = LoadTGA("Image//Soil.tga");

	//Crosshairs
	meshList[CObjectClass::GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("Crosshair",Color (1,1,1), 1);
	meshList[CObjectClass::GEO_CROSSHAIR]->textureID[0] = LoadTGA("Image//crosshair.tga");

	//Health
	meshList[CObjectClass::GEO_HEALTH] = MeshBuilder::GenerateQuad("Health Bar",Color (1,1,1), 1);
	meshList[CObjectClass::GEO_HEALTH]->textureID[0] = LoadTGA("Image//healthbar.tga");

	//Oil Drum
	meshList[CObjectClass::GEO_OIL_DRUM] = MeshBuilder::GenerateOBJ("OilDrum", "OBJ//oildrum.obj");
	meshList[CObjectClass::GEO_OIL_DRUM]->textureID[0] = LoadTGA("Image//oildrum.tga");
	meshList[CObjectClass::GEO_OIL_DRUM]->textureID[1] = LoadTGA("Image//Rust.tga");

	newObjectPos.Set(-20, 0 + terrainSize.y * ReadHeightMap(m_heightMap, -20/terrainSize.x, -20/terrainSize.z), -20);
	newObjectRot.SetToIdentity();
	newObjectScale.Set(1,1,1);
	newObjectSize.Set(7.19f,7.92f,7.19f);

	newObject = new CObjectClass(newObjectPos,newObjectScale,newObjectRot,newObjectSize,CObjectClass::GEO_OIL_DRUM);

	objectList.push_back(newObject);

	//Metal Shack
	meshList[CObjectClass::GEO_SHACK] = MeshBuilder::GenerateOBJ("Shack", "OBJ//Shack.obj");
	meshList[CObjectClass::GEO_SHACK]->textureID[0] = LoadTGA("Image//Shack.tga");

	newObjectPos.Set(50, 0 + terrainSize.y * ReadHeightMap(m_heightMap, 50/terrainSize.x, 50/terrainSize.z), 50);
	newObjectRot.SetToIdentity();
	newObjectScale.Set(1,1,1);
	newObjectSize.Set(20.39f,23.92f,18.47f);

	newObject = new CObjectClass(newObjectPos,newObjectScale,newObjectRot,newObjectSize,CObjectClass::GEO_SHACK);

	objectList.push_back(newObject);

	//Building
	meshList[CObjectClass::GEO_BUILDING] = MeshBuilder::GenerateOBJ("Shack", "OBJ//building.obj");
	meshList[CObjectClass::GEO_BUILDING]->textureID[0] = LoadTGA("Image//building.tga");
	meshList[CObjectClass::GEO_BUILDING]->textureID[1] = LoadTGA("Image//Concrete.tga");

	newObjectPos.Set(-50, 0 + terrainSize.y * ReadHeightMap(m_heightMap, -50/terrainSize.x, -50/terrainSize.z), -50);
	newObjectRot.SetToIdentity();
	newObjectScale.Set(1,1,1);
	newObjectSize.Set(35.79f,63.05f,41.20f);

	newObject = new CObjectClass(newObjectPos,newObjectScale,newObjectRot,newObjectSize,CObjectClass::GEO_BUILDING);

	objectList.push_back(newObject);

	//Bobafett Target
	meshList[CObjectClass::GEO_BOBA_TARGET] = MeshBuilder::GenerateOBJ("Boba Target", "OBJ//targetboard.obj");
	meshList[CObjectClass::GEO_BOBA_TARGET]->textureID[0] = LoadTGA("Image//bobafett.tga");

	newObjectPos.Set(-120, 2 + terrainSize.y * ReadHeightMap(m_heightMap, -120/terrainSize.x, -40/terrainSize.z), -40);
	newObjectRot.SetToIdentity();
	newObjectScale.Set(1,1,1);
	newObjectSize.Set(8.8f,9.45f,0.37f);
	
	newObject = new CObjectClass(newObjectPos,newObjectScale,newObjectRot,newObjectSize,CObjectClass::GEO_BOBA_TARGET);

	objectList.push_back(newObject);

	newObjectPos.Set(60, -3 + terrainSize.y * ReadHeightMap(m_heightMap, 60/terrainSize.x, 10/terrainSize.z), 10);
	newObjectRot.SetToRotation(-90,0,1,0);
	newObjectScale.Set(1,1,1);
	newObjectSize.Set(8.8f,9.45f,0.37f);

	newObject = new CObjectClass(newObjectPos,newObjectScale,newObjectRot,newObjectSize,CObjectClass::GEO_BOBA_TARGET);

	objectList.push_back(newObject);

	//DarthVader Target
	meshList[CObjectClass::GEO_VADER_TARGET] = MeshBuilder::GenerateOBJ("Boba Target", "OBJ//targetboard.obj");
	meshList[CObjectClass::GEO_VADER_TARGET]->textureID[0] = LoadTGA("Image//darthvader.tga");

	newObjectPos.Set(0, 0 + terrainSize.y * ReadHeightMap(m_heightMap, 0/terrainSize.x, -100/terrainSize.z), -100);
	newObjectRot.SetToIdentity();
	newObjectScale.Set(1,1,1);
	newObjectSize.Set(8.8f,9.45f,0.37f);
	
	newObject = new CObjectClass(newObjectPos,newObjectScale,newObjectRot,newObjectSize,CObjectClass::GEO_VADER_TARGET);

	objectList.push_back(newObject);

	newObjectPos.Set(-25, -2 + terrainSize.y * ReadHeightMap(m_heightMap, -25/terrainSize.x, 150/terrainSize.z), 150);
	newObjectRot.SetToRotation(180,0,1,0);
	newObjectScale.Set(1,1,1);
	newObjectSize.Set(8.8f,9.45f,0.37f);
	
	newObject = new CObjectClass(newObjectPos,newObjectScale,newObjectRot,newObjectSize,CObjectClass::GEO_VADER_TARGET);

	objectList.push_back(newObject);

	newObjectPos.Set(-200, 1 + terrainSize.y * ReadHeightMap(m_heightMap, -200/terrainSize.x, 0/terrainSize.z), 0);
	newObjectRot.SetToRotation(90,0,1,0);
	newObjectScale.Set(1,1,1);
	newObjectSize.Set(8.8f,9.45f,0.37f);
	
	newObject = new CObjectClass(newObjectPos,newObjectScale,newObjectRot,newObjectSize,CObjectClass::GEO_VADER_TARGET);

	objectList.push_back(newObject);

	meshList[CObjectClass::GEO_AK] = MeshBuilder::GenerateOBJ("AK","OBJ//AK.obj");
	meshList[CObjectClass::GEO_AK]->textureID[0] = LoadTGA("Image//AK.tga");

	meshList[CObjectClass::GEO_DEAGLE] = MeshBuilder::GenerateOBJ("AK","OBJ//DEAGLE.obj");
	meshList[CObjectClass::GEO_DEAGLE]->textureID[0] = LoadTGA("Image//DEAGLE.tga");

	meshList[CObjectClass::GEO_BULLET] = MeshBuilder::GenerateSphere("bullet", Color(0, 0, 0), 18, 36, 1.f);

	//Fire Animation
	meshList[CObjectClass::GEO_SPRITE_ANIMATION] = MeshBuilder::GenerateSpriteAnimation("fire",1,5);
	meshList[CObjectClass::GEO_SPRITE_ANIMATION]->textureID[0] = LoadTGA("Image//fire.tga");

	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[CObjectClass::GEO_SPRITE_ANIMATION]);

	//Enemy
	meshList[CObjectClass::GEO_ENEMY] = MeshBuilder::GenerateSphere("enemy",Color (1, 0, 0),18, 36, 1.f);
	//meshList[CObjectClass::GEO_ENEMY]->textureID[0] = LoadTGA("Image//enemy.tga");

	meshList[CObjectClass::GEO_RAIN] = MeshBuilder::GenerateSphere("Rain",Color (0, 0, 1),18, 36, 1.f);

	if(sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0,4,0,1.f);
	}

	m_cMinimap = new CMinimap();
	m_cMinimap->SetBackground(MeshBuilder::GenerateQuad("Minimap", Color(1,1,1),1.f));
	m_cMinimap->GetBackground()->textureID[0] = LoadTGA("Image//minimap_grass.tga");
	m_cMinimap->GetBackground()->textureID[1] = LoadTGA("Image//minimap_soil.tga");
	
	m_cMinimap->SetBorder(MeshBuilder::GenerateQuad("MinimapBoarder", Color(1,1,1),1.f));
	m_cMinimap->GetBorder()->textureID[0] = LoadTGA("Image//minimap.tga");
	
	Avatar *temp = new Avatar;
	temp->setMesh((MeshBuilder::GenerateQuad("MinimapAvatar", Color(1,1,0), 1.f)));
	temp->getMesh()->textureID[0] = LoadTGA("Image//avatar.tga");
	temp->setPostion(60, -45);
	
	m_cMinimap->SetPlayer(temp);

	//meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	//meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 10.f);
	//meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", 1, 1, 1);
	//meshList[GEO_TORUS] = MeshBuilder::GenerateCylinder("torus", 36, 36, 5, 1);
	//meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	//meshList[GEO_CONE]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	//meshList[GEO_CONE]->material.kSpecular.Set(0.f, 0.f, 0.f);
	
	/*meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("LEFT", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID[0] = LoadTGA("Image//left.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("RIGHT", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID[0] = LoadTGA("Image//right.tga");
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("TOP", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID[0] = LoadTGA("Image//top.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("BOTTOM", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID[0] = LoadTGA("Image//bottom.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("FRONT", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID[0] = LoadTGA("Image//front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("BACK", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID[0] = LoadTGA("Image//back.tga");*/
}

void GenerateSkyPlane::InitParams()
{
	m_programID = LoadShaders( "Shader//Fog.vertexshader", "Shader//Fog.fragmentshader" );
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED0] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE0] = glGetUniformLocation(m_programID, "colorTexture[0]");
	m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
	m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	
	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID,"fogParam.color");
	m_parameters[U_FOG_START] = glGetUniformLocation(m_programID,"fogParam.start");
	m_parameters[U_FOG_END] = glGetUniformLocation(m_programID,"fogParam.end");
	m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID,"fogParam.density");
	m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID,"fogParam.type");
	m_parameters[U_FOG_ENABLE] = glGetUniformLocation(m_programID,"fogParam.enabled");

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 350, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	//lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_POINT;
	lights[1].position.Set(0, 400, 0);
	lights[1].color.Set(1, 1, 0.5f);
	lights[1].power = 20.f;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[1].cosInner = cos(Math::DegreeToRadian(30));
	lights[1].exponent = 3.f;
	lights[1].spotDirection.Set(0.f, 1.f, 0.f);
	
	numLights = 1;

	glUniform1i(m_parameters[U_NUMLIGHTS], numLights);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	for(int i = 0; i < numLights; ++i)
	{
		glUniform1i(m_parameters[i * 11 + 7], lights[i].type);
		glUniform3fv(m_parameters[i * 11 + 9], 1, &lights[i].color.r);
		glUniform1f(m_parameters[i * 11 + 10], lights[i].power);
		glUniform1f(m_parameters[i * 11 + 11], lights[i].kC);
		glUniform1f(m_parameters[i * 11 + 12], lights[i].kL);
		glUniform1f(m_parameters[i * 11 + 13], lights[i].kQ);
		glUniform1f(m_parameters[i * 11 + 15], lights[i].cosCutoff);
		glUniform1f(m_parameters[i * 11 + 16], lights[i].cosInner);
		glUniform1f(m_parameters[i * 11 + 17], lights[i].exponent);
	}

	Color fogcolor = (0.5f,0.5f,0.5f);
	glUniform3fv(m_parameters[U_FOG_COLOR],1, &fogcolor.r);
	glUniform1f(m_parameters[U_FOG_START], 10);
	glUniform1f(m_parameters[U_FOG_END], 1000);
	glUniform1f(m_parameters[U_FOG_DENSITY], 0.005f);
	glUniform1i(m_parameters[U_FOG_TYPE], 1);
	glUniform1i(m_parameters[U_FOG_ENABLE], 0);
}

void GenerateSkyPlane::InitSounds()
{
	//start the sound engine with default parameters
	theSoundEngine = createIrrKlangDevice();
	gunSound = NULL;
	walkSound = NULL;
	SND[SND_WALKING] = theSoundEngine->addSoundSourceFromFile("wav//walking.wav");
	SND[SND_WALKING]->setDefaultVolume(0.5f);
	SND[SND_GUNFIRE_AK] = theSoundEngine->addSoundSourceFromFile("wav//gunfireAK.wav");
	SND[SND_GUNFIRE_DEAGLE] = theSoundEngine->addSoundSourceFromFile("wav//gunfireDEAGLE.wav");
	SND[SND_RELOAD] = theSoundEngine->addSoundSourceFromFile("wav//reload.wav");
}