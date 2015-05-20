#include "GenerateSkyPlane.h"

void GenerateSkyPlane::InitObjects()
{
	for(int i = 0; i < CObjectClass::NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[CObjectClass::GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[CObjectClass::GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("Crosshair",Color(1,1,1),5);
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

	meshList[CObjectClass::GEO_OIL_DRUM] = MeshBuilder::GenerateOBJ("OilDrum", "OBJ//oildrum.obj");
	meshList[CObjectClass::GEO_OIL_DRUM]->textureID[0] = LoadTGA("Image//oildrum.tga");
	meshList[CObjectClass::GEO_OIL_DRUM]->textureID[1] = LoadTGA("Image//Rust.tga");

	newObjectPos.Set(-20, 2 + terrainSize.y * ReadHeightMap(m_heightMap, -20/terrainSize.x, -20/terrainSize.z), -20);
	newObjectRot.SetToIdentity();
	newObjectScale.Set(1,1,1);
	newObject = new CObjectClass(newObjectPos,newObjectScale,newObjectRot,7.2f,CObjectClass::GEO_OIL_DRUM);

	objectList.push_back(newObject);

	meshList[CObjectClass::GEO_BULLET] = MeshBuilder::GenerateSphere("bullet", Color(1, 1, 1), 18, 36, 1.f);

	meshList[CObjectClass::GEO_SPRITE_ANIMATION] = MeshBuilder::GenerateSpriteAnimation("cat",1,6);
	meshList[CObjectClass::GEO_SPRITE_ANIMATION]->textureID[0] = LoadTGA("Image//cat.tga");

	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[CObjectClass::GEO_SPRITE_ANIMATION]);

	if(sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0,4,0,1.f);
	}

	meshList[CObjectClass::GEO_SHACK] = MeshBuilder::GenerateOBJ("Shack", "OBJ//Shack.obj");
	meshList[CObjectClass::GEO_SHACK]->textureID[0] = LoadTGA("Image//Shack.tga");

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

void GenerateSkyPlane::InitLights()
{
	m_programID = LoadShaders( "Shader//comg.vertexshader", "Shader//MultiTexture.fragmentshader" );
	
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
	
	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(1, 1, 0);
	lights[1].color.Set(1, 1, 0.5f);
	lights[1].power = 0.4f;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[1].cosInner = cos(Math::DegreeToRadian(30));
	lights[1].exponent = 3.f;
	lights[1].spotDirection.Set(0.f, 1.f, 0.f);
	
	numLights = 2;

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
}