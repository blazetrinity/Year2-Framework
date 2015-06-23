#include "GenerateSkyPlane.h"

void GenerateSkyPlane::RenderLights()
{
	for(int i = 0; i <	numLights; ++i)
	{
		if(lights[i].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(lights[i].position.x, lights[i].position.y, lights[i].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[i * 11 + 8], 1, &lightDirection_cameraspace.x);
		}
		else if(lights[i].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * lights[i].position;
			glUniform3fv(m_parameters[i * 11 + 8], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * lights[i].spotDirection;
			glUniform3fv(m_parameters[i * 11 + 14], 1, &spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() * lights[i].position;
			glUniform3fv(m_parameters[i * 11 + 8], 1, &lightPosition_cameraspace.x);
		}


		modelStack.PushMatrix();
		modelStack.Translate(lights[i].position.x, lights[i].position.y, lights[i].position.z);
		RenderMesh(meshList[CObjectClass::GEO_LIGHTBALL], false);
		modelStack.PopMatrix();
	}
}
	
void GenerateSkyPlane::RenderEnvironment()
{
	RenderMesh(meshList[CObjectClass::GEO_AXES], false);

	// perspective;
	////perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	//projectionStack.LoadMatrix(perspective);
	//viewStack.LoadIdentity();
	//
	//modelStack.PushMatrix();
	////modelStack.Translate(20, 0, -20);
	////modelStack.Scale(0.1f, 0.1f, 0.1f);
	//modelStack.Scale(50, 50, 50);
	////RenderMesh(meshList[GEO_QUAD], false);
	//RenderText(meshList[GEO_TEXT], "HelloWorld", Color(0, 1, 0));
	//modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(0, 48, 0);
	modelStack.Rotate(-90,1,0,0);
	modelStack.Scale(terrainSize.x,terrainSize.z,1);
	RenderMesh(meshList[CObjectClass::GEO_POND], false);
	modelStack.PopMatrix();
}

void GenerateSkyPlane::RenderAi()
{
	for(std::vector<CAi *>::iterator it = AiList.begin(); it != AiList.end(); ++it)
	{
		CAi *obj = (CAi *)*it;
		if(obj->getState() == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(obj->getTranslate().x, obj->getTranslate().y, obj->getTranslate().z);
			modelStack.MultMatrix(obj->getRotate());
			modelStack.Scale(obj->getScale().x,obj->getScale().y,obj->getScale().z);
			RenderMesh(meshList[obj->getID()],false);
			modelStack.PopMatrix();
		}
	}
}

void GenerateSkyPlane::RenderSprite()
{
	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[CObjectClass::GEO_SPRITE_ANIMATION]);

	modelStack.PushMatrix();
	modelStack.Translate(-20, 12 + terrainSize.y * ReadHeightMap(m_heightMap, -20/terrainSize.x, -20/terrainSize.z), -20);
	modelStack.Scale(10,10,1);
	RenderMesh(sa, false);
	modelStack.PopMatrix();
}
	
void GenerateSkyPlane::RenderObjects()
{
	for(std::vector<CObjectClass *>::iterator it = objectList.begin(); it != objectList.end(); ++it)
	{
		CObjectClass *obj = (CObjectClass *)*it;
		modelStack.PushMatrix();
		modelStack.Translate(obj->getTranslate().x,obj->getTranslate().y,obj->getTranslate().z);
		modelStack.MultMatrix(obj->getRotate());
		modelStack.Scale(obj->getScale().x,obj->getScale().y,obj->getScale().z);
		RenderMesh(meshList[obj->getID()],false);
		modelStack.PopMatrix();
	}
}

void GenerateSkyPlane::RenderCharacter()
{
	modelStack.PushMatrix();
	modelStack.Translate(m_Player.GetCamera().position.x, m_Player.GetCamera().position.y, m_Player.GetCamera().position.z);
	modelStack.Rotate(m_Player.GetCamera().totalyaw, 0, 1, 0);
	modelStack.Rotate(m_Player.GetCamera().totalpitch, 1 , 0, 0);

	modelStack.PushMatrix();
	modelStack.Translate(m_Player.GetCurrentWeapon()->getTranslate().x, m_Player.GetCurrentWeapon()->getTranslate().y, m_Player.GetCurrentWeapon()->getTranslate().z);
	modelStack.Scale(m_Player.GetCurrentWeapon()->getScale().x, m_Player.GetCurrentWeapon()->getScale().y, m_Player.GetCurrentWeapon()->getScale().z);
	RenderMesh(meshList[m_Player.GetCurrentWeapon()->getID()], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}

void GenerateSkyPlane::RenderHUD()
{
	SetHUD(true);

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[CObjectClass::GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 55);
	
	RenderMeshIn2D(meshList[CObjectClass::GEO_HEALTH], false, 30.f, -60, -45);

	std::ostringstream ss1;
	ss1 << m_Player.GetHealth();
	RenderTextOnScreen(meshList[CObjectClass::GEO_TEXT], ss1.str(), Color(1, 0, 0), 3, 6, 6.5f);

	std::ostringstream ss2;
	ss2 << m_Player.GetCurrentWeapon()->GetcurrentBullets() << "/";
	RenderTextOnScreen(meshList[CObjectClass::GEO_TEXT], ss2.str(), Color(0, 1, 0), 3, 32, 7.5f);

	std::ostringstream ss3;
	ss3 << m_Player.GetCurrentWeapon()->GetnumBulletsTotal();
	RenderTextOnScreen(meshList[CObjectClass::GEO_TEXT], ss3.str(), Color(0, 1, 0), 3, 40, 7.5f);

	RenderMeshIn2D(meshList[CObjectClass::GEO_CROSSHAIR], false, 17.5f, 0, 0);

	modelStack.PushMatrix();
	modelStack.Translate(60,-45,0);

	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle,0,0,1);

	modelStack.PushMatrix();
	modelStack.Scale(30,30,1);
	RenderMeshIn2D(m_cMinimap->GetBackground(), false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(30,30,1);
	RenderMeshIn2D(m_cMinimap->GetBorder(), false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle,0,0,1);

	for(int i = 0; i < m_cMinimap->GetEnemyList().size(); ++i)
	{
		if(m_cMinimap->GetEnemyList()[i]->getRender())
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_cMinimap->GetEnemyList()[i]->getPosition_x() ,-m_cMinimap->GetEnemyList()[i]->getPosition_y(), 0);
			modelStack.Scale(3,3,1);
			RenderMeshIn2D(m_cMinimap->GetEnemyList()[i]->getMesh(),false);
			modelStack.PopMatrix();
		}
	}

	modelStack.PopMatrix();

	modelStack.Scale(3,3,1);
	RenderMeshIn2D(m_cMinimap->GetPlayer()->getMesh(), false);
	modelStack.PopMatrix();

	SetHUD(false);
}

static const float SKYBOXSIZE = 1000.f;

void GenerateSkyPlane::RenderSkybox()
{
//	//left
//	modelStack.PushMatrix();
//	modelStack.Rotate(90, 0, 1, 0);
//	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
//	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
//	RenderMesh(meshList[GEO_LEFT], false);
//	modelStack.PopMatrix();
//	
//	modelStack.PushMatrix();
//	modelStack.Rotate(-90, 0, 1, 0);
//	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
//	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
//	RenderMesh(meshList[GEO_RIGHT], false);
//	modelStack.PopMatrix();
//	
//	modelStack.PushMatrix();
//	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
//	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
//	RenderMesh(meshList[GEO_FRONT], false);
//	modelStack.PopMatrix();
//	
//	modelStack.PushMatrix();
//	modelStack.Rotate(180, 0, 1, 0);
//	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
//	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
//	RenderMesh(meshList[GEO_BACK], false);
//	modelStack.PopMatrix();
//	
//	modelStack.PushMatrix();
//	modelStack.Rotate(90, 1, 0, 0);
//	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
//	modelStack.Rotate(90, 0, 0, 1);
//	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
//	RenderMesh(meshList[GEO_TOP], false);
//	modelStack.PopMatrix();
//	
//	modelStack.PushMatrix();
//	modelStack.Rotate(-90, 1, 0, 0);
//	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
//	modelStack.Rotate(-90, 0, 0, 1);
//	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
//	RenderMesh(meshList[GEO_BOTTOM], false);
//	modelStack.PopMatrix();
}

void GenerateSkyPlane::RenderSkyPlane()
{
	modelStack.PushMatrix();
	modelStack.Translate(0,1800,-0);
	RenderMesh(meshList[CObjectClass::GEO_SKYPLANE],false);
	modelStack.PopMatrix();
}

void GenerateSkyPlane::RenderTerrain()
{
	modelStack.PushMatrix();
	modelStack.Scale(terrainSize.x,terrainSize.y,terrainSize.z);
	RenderMesh(meshList[CObjectClass::GEO_TERRAIN],false);
	modelStack.PopMatrix();
}

void GenerateSkyPlane::RenderBullet()
{
	for(int i = 0; i < bulletList.size(); ++i)
	{
		if(bulletList[i]->GetStatus() == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(bulletList[i]->getTranslate().x,bulletList[i]->getTranslate().y,bulletList[i]->getTranslate().z);
			modelStack.Scale(bulletList[i]->getScale().x,bulletList[i]->getScale().y,bulletList[i]->getScale().z);
			RenderMesh(meshList[bulletList[i]->getID()], false);
			modelStack.PopMatrix();
		}
	}
}