#ifndef GENERATESKYPLANE_H
#define GENERATESKYPLANE_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "LoadHmap.h"
#include "BulletInfo.h"
#include "Minimap.h"
#include "Character.h"
#include "Weapon.h"
#include "Ai.h"
#include "Particle.h"
#include <irrKlang.h>
using namespace irrklang;
#include <sstream>
#include <vector>

#pragma comment(lib, "irrKlang.lib")

using std::vector;

class GenerateSkyPlane : public Scene
{
public:
	GenerateSkyPlane();
	~GenerateSkyPlane();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void UpdateCameraStatus(const unsigned char key);
	virtual void UpdateWeaponStatus(const unsigned char key);
	virtual void Exit();

	virtual void StartGame();

	void InitObjects();
	void InitParams();
	void InitSounds();

	void SetHUD(const bool m_bHUDmode);

	void GameUpdate(double dt);

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);	
	void RenderMeshIn2D(Mesh* mesh, bool enableLight);
	void RenderMeshIn2D(Mesh* mesh, bool enableLight, float size, float x = 0.0f, float y = 0.0f, bool rotate = false);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderSkybox();
	void RenderSkyPlane();
	void RenderTerrain();
	void RenderLights();
	void RenderEnvironment();
	void RenderSprite();
	void RenderAi();
	void RenderHUD();
	void RenderBullet();
	void RenderObjects();
	void RenderCharacter();

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,

		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHTENABLED,
		U_NUMLIGHTS,

		U_COLOR_TEXTURE0,
		U_COLOR_TEXTURE_ENABLED0,
		U_COLOR_TEXTURE1,
		U_COLOR_TEXTURE_ENABLED1,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_FOG_COLOR,
		U_FOG_START,
		U_FOG_END,
		U_FOG_DENSITY,
		U_FOG_TYPE,
		U_FOG_ENABLE,

		U_TOTAL,
	};

	enum WEAPON_ACTION
	{
		WA_NIL = 0,
		WA_FIRE,
		WA_RELOAD,
		WA_CHANGEWEAPON_1,
		WA_CHANGEWEAPON_2,
		WA_TOTAL,
	};

	enum SOUNDS
	{
		SND_WALKING = 0,
		SND_GUNFIRE_AK,
		SND_GUNFIRE_DEAGLE,
		SND_RELOAD,
		TOTAL_SND,
	};

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[CObjectClass::NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float rotateAngle;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[2];

	vector<unsigned char> m_heightMap;

	Vector3 terrainSize;

	int numLights;

	bool bLightEnabled;
	bool m_bReload;
	bool m_bFire;
	bool m_bRecoil;
	bool m_startgame;
	bool m_currentgame;
	bool m_endgame;

	float fps;

	float m_fLastshot;
	float m_fOldpitch;
	float m_fNewpitch;

	int m_AiCount;

	vector<CBulletInfo*> bulletList;
	vector<CObjectClass*> objectList;
	vector<CAi*> AiList;
	vector<CParticle*> ParticleList;
	CObjectClass* newObject;
	Vector3 newObjectPos, newObjectScale, newObjectSize;
	Mtx44 newObjectRot;

	CMinimap* m_cMinimap;

	CCharacter m_Player;

	//The sound engine
	ISoundEngine* theSoundEngine;
	ISound* gunSound;
	ISound* walkSound;
	ISoundSource* SND[TOTAL_SND];
};

#endif