#include "GenerateSkyPlane.h"


GenerateSkyPlane::GenerateSkyPlane() : m_cMinimap(NULL), theSoundEngine(NULL)
{
}

GenerateSkyPlane::~GenerateSkyPlane()
{
	if(m_cMinimap)
	{
		delete m_cMinimap;
		m_cMinimap = NULL;
	}

	if(theSoundEngine != NULL)
	{
		theSoundEngine->drop();
	}
}

void GenerateSkyPlane::Init()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	InitParams();
	
	terrainSize.Set(4000,350,4000);

	InitObjects();

	InitSounds();

	m_Player.Init(Vector3(0, 10 + terrainSize.y * ReadHeightMap(m_heightMap, 0/terrainSize.x, 10/terrainSize.z), 10), Vector3(0, 10 + terrainSize.y * ReadHeightMap(m_heightMap, 0/terrainSize.x, 10/terrainSize.z), 0), Vector3(0, 1, 0));
	
	Vector3 Pos, Scale;
	Mtx44 Rotation;

	CWeapon* weapon;

	Pos.Set(1,-3.5,-3.5);
	Scale.Set(1,1,1);
	Rotation.SetToIdentity();

	weapon = new CWeapon;
	weapon->Init(0.1f, 1.f, 30, 30, 300, 420, Pos, Scale, Rotation, CObjectClass::GEO_AK);
	m_Player.AddWeapon(*weapon);

	Pos.Set(1,-3,-3.5);

	weapon = new CWeapon;
	weapon->Init(0.3f, 0.5f, 8, 8, 999, 999, Pos, Scale, Rotation, CObjectClass::GEO_DEAGLE);
	m_Player.AddWeapon(*weapon);

	for(int i = 0; i < 10; ++i)
	{
		CBulletInfo *b = new CBulletInfo;
		bulletList.push_back(b);
	}

	for(int i = 0; i < 15; ++i)
	{
		CAi *a = new CAi;
		AiList.push_back(a);
	}

	for(int i = 0; i < 1000; ++i)
	{
		float x = Math::RandFloatMinMax(-2000,2000);
		float z = Math::RandFloatMinMax(-2000,2000);
		Vector3 randPos, Scale;
		randPos.Set(x, 1000,z);
		Scale.Set(2,2,2);
		Mtx44 Rotate;
		Rotate.SetToIdentity();

		CParticle *p = new CParticle(randPos,Scale,Vector3(0,-150,0),Rotate,Vector3(0,0,0),CObjectClass::GEO_RAIN,true,true);
		ParticleList.push_back(p);
	}

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	rotateAngle = 0;

	bLightEnabled = true;
	m_bReload = false;
	m_bFire = false;
	m_bRecoil = false;
	m_startgame = false;
	m_currentgame = false;
	m_endgame = false;

	m_fLastshot = 0;
	m_fOldpitch = 0;
	m_fNewpitch = 0;

	m_AiCount = 0;
}

void GenerateSkyPlane::UpdateCameraStatus(const unsigned char key)
{
	if(key == 'a' || key == 's' || key == 'd' || key == 'w')
	{
		if(walkSound == NULL)
		{
			walkSound = theSoundEngine->play2D(SND[SND_WALKING],false,true);
		}
		else if(walkSound->isFinished() == true)
		{
			walkSound = theSoundEngine->play2D(SND[SND_WALKING],false,true);
		}
	}
	m_Player.UpdateCameraStatus(key);
}

void GenerateSkyPlane::UpdateWeaponStatus(const unsigned char key)
{
	if(key == WA_FIRE && m_fLastshot > m_Player.GetCurrentWeapon()->GetFirerate())
	{
		if((m_Player.GetCurrentWeapon()->GetcurrentBullets() > 0) && (m_bReload == false))
		{
			if(m_Player.GetCurrentWeapon()->getID() == CObjectClass::GEO_AK)
			{
				gunSound = theSoundEngine->play2D(SND[SND_GUNFIRE_AK],false,true);
			}
		
			else
			{
				gunSound = theSoundEngine->play2D(SND[SND_GUNFIRE_DEAGLE],false,true);
			}

			for(int i = 0; i < bulletList.size(); ++i)
			{
				if(bulletList[i]->GetStatus() == false)
				{
					bulletList[i]->Init(m_Player.GetCamera().position,(m_Player.GetCamera().target - m_Player.GetCamera().position).Normalized(),Vector3(1,1,1),Mtx44(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),Vector3 (2,1,2),CObjectClass::GEO_BULLET,600.f,5.f);
					m_Player.GetCurrentWeapon()->Fire();
					m_fLastshot = 0;
					m_bFire = true;
					break;
				}

				if(i == bulletList.size()-1)
				{
					for(int i = 0; i < 10; ++i)
					{
						CBulletInfo *b = new CBulletInfo;
						bulletList.push_back(b);
					}
					bulletList[i+1]->Init(m_Player.GetCamera().position,(m_Player.GetCamera().target - m_Player.GetCamera().position).Normalized(),Vector3(1,1,1),Mtx44(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0),Vector3 (2,1,2),CObjectClass::GEO_BULLET,600.f,5.f);
					m_Player.GetCurrentWeapon()->Fire();
					m_fLastshot = 0;
					m_bFire = true;
					break;
				}
			}
		}
	}

	else if(key == WA_RELOAD)
	{
		m_bReload = true;
		gunSound = theSoundEngine->play2D(SND[SND_RELOAD],false,true);
	}

	else if(key == WA_CHANGEWEAPON_1 && (m_Player.GetCurrentWeapon()->getID() != CObjectClass::GEO_AK) && m_bReload == false)
	{
		m_Player.ChangeWeapon(0);
	}

	else if(key == WA_CHANGEWEAPON_2 && (m_Player.GetCurrentWeapon()->getID() != CObjectClass::GEO_DEAGLE) && m_bReload == false)
	{
		m_Player.ChangeWeapon(1);
	}

	if(gunSound != NULL)
	{
		if(gunSound->getIsPaused()== true)
		{
			gunSound->setIsPaused(false);
		}
		else if(gunSound->isFinished() == true)
		{
			gunSound->drop();
			gunSound = NULL;
		}
	}
}


void GenerateSkyPlane::Update(double dt)
{
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(Application::IsKeyPressed('5'))
	{
		lights[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('6'))
	{
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('7'))
	{
		lights[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	else if(Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}

	if(Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(10.f * dt);
	if(Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(10.f * dt);
	if(Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(10.f * dt);
	if(Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(10.f * dt);
	if(Application::IsKeyPressed('O'))
		lights[0].position.y -= (float)(10.f * dt);
	if(Application::IsKeyPressed('P'))
		lights[0].position.y += (float)(10.f * dt);

	rotateAngle += Application::camera_yaw * 200 * (float)dt; // += (float)(10 * dt);

	//Update bullets
	for(int i = 0; i < bulletList.size(); ++i)
	{
		bulletList[i]->Update(dt);
	}

	//CheckBulletCollision
	for(int i = 0; i < bulletList.size(); ++i)
	{
		if(bulletList[i]->GetStatus() == true)
		{
			if(bulletList[i]->getTranslate().y < terrainSize.y * ReadHeightMap(m_heightMap, bulletList[i]->getTranslate().x/terrainSize.x, bulletList[i]->getTranslate().z/terrainSize.z))
			{
				bulletList[i]->SetStatus(false);
			}

			for(int j = 0; j < objectList.size(); ++j)
			{
				if(bulletList[i]->getBoundMin().x < objectList[j]->getBoundMax().x && bulletList[i]->getBoundMax().x > objectList[j]->getBoundMin().x 
				&& bulletList[i]->getBoundMin().y < objectList[j]->getBoundMax().y && bulletList[i]->getBoundMax().y > objectList[j]->getBoundMin().y 
				&& bulletList[i]->getBoundMin().z < objectList[j]->getBoundMax().z && bulletList[i]->getBoundMax().z > objectList[j]->getBoundMin().z && bulletList[i]->GetStatus() == true)
				{
					bulletList[i]->SetStatus(false);
				}
			}

			if(m_currentgame == true)
			{
				for(int j = 0; j < AiList.size(); ++j)
				{
					if(bulletList[i]->getBoundMin().x < AiList[j]->getBoundMax().x && bulletList[i]->getBoundMax().x > AiList[j]->getBoundMin().x 
					&& bulletList[i]->getBoundMin().y < AiList[j]->getBoundMax().y && bulletList[i]->getBoundMax().y > AiList[j]->getBoundMin().y 
					&& bulletList[i]->getBoundMin().z < AiList[j]->getBoundMax().z && bulletList[i]->getBoundMax().z > AiList[j]->getBoundMin().z && AiList[j]->getState() == true && bulletList[i]->GetStatus() == true)
					{
						AiList[j]->setState(false);
						m_cMinimap->GetEnemyList()[j]->setRender(false);
						bulletList[i]->SetStatus(false);
						--m_AiCount;
						break;
					}
				}
			}
		}
	}

	for(int i = 0; i < ParticleList.size(); ++i)
	{
		if(ParticleList[i]->getstatus() == true)
		{
			ParticleList[i]->Update(dt);

			if(ParticleList[i]->getID() == CObjectClass::GEO_RAIN && ParticleList[i]->getTranslate().y <= terrainSize.y * ReadHeightMap(m_heightMap, ParticleList[i]->getTranslate().x/terrainSize.x, ParticleList[i]->getTranslate().z/terrainSize.z))
			{
				ParticleList[i]->setstatus(false);
			}
		}

		else if(ParticleList[i]->getstatus() == false)
		{
			if(ParticleList[i]->getID() == CObjectClass::GEO_RAIN)
			{
				float x = Math::RandFloatMinMax(-2000,2000);
				float z = Math::RandFloatMinMax(-2000,2000);
				Vector3 randPos;
				randPos.Set(x,1000,z);

				ParticleList[i]->setTranslate(randPos);
				ParticleList[i]->setstatus(true);
				ParticleList[i]->setvel(Vector3(0,-150,0));
			}
		}
	}

	//GameUpdate - Ai movement spawning and dying
	GameUpdate(dt);

	//Gun Reload
	static float fReloadtimer = 0;

	if(m_bReload)
	{
		fReloadtimer += 1 * dt;
	}

	if(fReloadtimer > m_Player.GetCurrentWeapon()->GetReloadspeed())
	{
		m_Player.GetCurrentWeapon()->Reload();
		fReloadtimer = 0;
		m_bReload = false;
	}

	//Gun Recoil
	if(m_bFire)
	{
		m_bRecoil = true;
	}

	else if(m_bFire == false && m_bRecoil == false)
	{
		m_fOldpitch = Application::camera_pitch;
	}

	if(m_bRecoil && m_bFire)
	{
		Application::camera_pitch -= 20 * dt;
		m_fNewpitch += Application::camera_pitch;
	}

	else if(m_bFire == false && m_fOldpitch > m_fNewpitch)
	{
		Application::camera_pitch += 1 * dt;
		m_fNewpitch += Application::camera_pitch;
	}

	if(m_fOldpitch <= m_fNewpitch)
	{
		m_bRecoil = false;
	}

	m_bFire = false;

	m_fLastshot += 1 * dt;

	//Update Camera
	m_Player.UpdateCamera(dt,m_heightMap,terrainSize);
	
	//SpriteAnimation Update
	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[CObjectClass::GEO_SPRITE_ANIMATION]);

	if(sa)
	{
		sa->Update(dt);
	}

	//Walking sound Update
	if(walkSound != NULL)
	{
		if(walkSound->getIsPaused() == true)
		{
			walkSound->setIsPaused(false);
		}
		else if(walkSound->isFinished() == true)
		{
			walkSound->drop();
			walkSound = NULL;
		}
	}

	fps = (float)(1.f / dt);
}

void GenerateSkyPlane::StartGame()
{
	if(m_startgame == false)
	{
		m_startgame = true;
	}
}

void GenerateSkyPlane::GameUpdate(double dt)
{
	if(m_startgame == true)
	{
		for(int i = 0; i < AiList.size(); ++i)
		{
			float x = Math::RandFloatMinMax(-2000,2000);
			float z = Math::RandFloatMinMax(-2000,2000);
			Vector3 randPos, Scale;
			randPos.Set(x, 10 + terrainSize.y * ReadHeightMap(m_heightMap, x/terrainSize.x, z/terrainSize.z),z);
			Scale.Set(10,10,10);
			Mtx44 Rotate;
			Rotate.SetToIdentity();
			AiList[i]->setState(true);
			AiList[i]->Init(randPos,Scale,Rotate,Vector3(2,1,2),CObjectClass::GEO_ENEMY);
			++m_AiCount;


			Avatar* enemy = new Avatar;
			enemy->setPostion(((x/terrainSize.x) * 30) - ((m_Player.GetCamera().position.x/terrainSize.x) * 30), ((z/terrainSize.z) * 30)- ((m_Player.GetCamera().position.z/terrainSize.z) * 30));
			enemy->setMesh((MeshBuilder::GenerateQuad("MinimapAvatar", Color(1,1,0), 1.f)));
			enemy->getMesh()->textureID[0] = LoadTGA("Image//avatar.tga");
			m_cMinimap->AddEnemyList(enemy);
		}
		m_startgame = false;
		m_currentgame = true;
	}

	if(m_currentgame == true)
	{
		for(int i = 0; i < AiList.size(); ++i)
		{
			if(AiList[i]->getState() == true)
			{
				AiList[i]->Update(m_Player.GetCamera().position,dt,terrainSize, &m_heightMap);
				m_cMinimap->GetEnemyList()[i]->setPostion(((AiList[i]->getTranslate().x/terrainSize.x) * 30) - ((m_Player.GetCamera().position.x/terrainSize.x) * 30), ((AiList[i]->getTranslate().z/terrainSize.z) * 30) - ((m_Player.GetCamera().position.z/terrainSize.z) * 30));

				if((m_Player.GetCamera().position - AiList[i]->getTranslate()).Length() < 200)
				{
					m_cMinimap->GetEnemyList()[i]->setRender(true);
				}

				else
				{
					m_cMinimap->GetEnemyList()[i]->setRender(false);
				}
			}
		}
	}

	if((m_Player.GetHealth() == 0 || m_AiCount == 0) && m_currentgame == true)
	{
		m_currentgame = false;
		m_endgame = true;
	}

	if(m_endgame == true)
	{
		m_Player.SetHealth(100);
		m_endgame = false;
	}
}

//Toggle HUD mode
void GenerateSkyPlane::SetHUD(const bool m_bHUDmode)
{
	if(m_bHUDmode)
	{
		glDisable(GL_DEPTH_TEST);
		Mtx44 ortho;
		ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
		projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
	}

	else
	{
		projectionStack.PopMatrix();
		glEnable(GL_DEPTH_TEST);
	}
}

void GenerateSkyPlane::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0)
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED0], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE0], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void GenerateSkyPlane::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID[0] <= 0)
		return;
	
	/*glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);*/
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED0], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE0], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	//projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	//glEnable(GL_DEPTH_TEST);
}

void GenerateSkyPlane::RenderMeshIn2D(Mesh*mesh, bool enableLight, float size, float x, float y, bool rotate)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80,80,-60,60,-10,10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();

	modelStack.LoadIdentity();
	modelStack.Translate(x,y,0);
	if(rotate)
	{
		modelStack.Rotate(rotateAngle,0,0,1);
	}
	
	modelStack.Scale(size,size,size);
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP],1,GL_FALSE, &MVP.a[0]);


	for(int i = 0; i < Mesh::MAX_TEXTURE; ++i)
	{
		if(mesh->textureID[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED0 + (i * 2)], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureID[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE0 + (i * 2)], i);
		}
	
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED0 + (i * 2)], 0);
		}
	}

	mesh->Render();

	for(int i = 0; i < Mesh::MAX_TEXTURE; ++i)
	{
		if(mesh->textureID[i] > 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void GenerateSkyPlane::RenderMeshIn2D(Mesh*mesh, bool enableLight)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80,80,-60,60,-10,10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP],1,GL_FALSE, &MVP.a[0]);


	for(int i = 0; i < Mesh::MAX_TEXTURE; ++i)
	{
		if(mesh->textureID[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED0 + (i * 2)], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureID[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE0 + (i * 2)], i);
		}
	
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED0 + (i * 2)], 0);
		}
	}

	mesh->Render();

	for(int i = 0; i < Mesh::MAX_TEXTURE; ++i)
	{
		if(mesh->textureID[i] > 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void GenerateSkyPlane::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if(enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);

		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);
		
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	for(int i = 0; i < Mesh::MAX_TEXTURE; ++i)
	{
		if(mesh->textureID[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED0 + (i * 2)], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureID[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE0 + (i * 2)], i);
		}
	
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED0 + (i * 2)], 0);
		}
	}
	mesh->Render();
	for(int i = 0; i < Mesh::MAX_TEXTURE; ++i)
	{
		if(mesh->textureID[i] > 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

void GenerateSkyPlane::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						m_Player.GetCamera().position.x, m_Player.GetCamera().position.y, m_Player.GetCamera().position.z,
						m_Player.GetCamera().target.x, m_Player.GetCamera().target.y, m_Player.GetCamera().target.z,
						m_Player.GetCamera().up.x, m_Player.GetCamera().up.y, m_Player.GetCamera().up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	RenderLights();
	
	//RenderSkybox();

	RenderSkyPlane();

	RenderTerrain();

	RenderEnvironment();

	RenderObjects();

	RenderAi();

	RenderCharacter();

	RenderSprite();

	RenderBullet();

	for(std::vector<CParticle *>::iterator it = ParticleList.begin(); it != ParticleList.end(); ++it)
	{
		CParticle *Particle = (CParticle *)*it;
		if(Particle->getstatus() == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(Particle->getTranslate().x,Particle->getTranslate().y,Particle->getTranslate().z);
			modelStack.MultMatrix(Particle->getRotate());
			modelStack.Scale(Particle->getScale().x,Particle->getScale().y,Particle->getScale().z);
			RenderMesh(meshList[Particle->getID()],false);
			modelStack.PopMatrix();
		}
	}

	RenderHUD();
}

void GenerateSkyPlane::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < CObjectClass::NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}

	for(int i = 0; i < bulletList.size(); ++i)
	{
		delete bulletList[i];
	}

	for(int i = 0; i < objectList.size(); ++i)
	{
		delete objectList[i];
	}

	for(int i = 0; i < AiList.size(); ++i)
	{
		delete AiList[i];
	}

	for(int i = 0; i < m_cMinimap->GetEnemyList().size(); ++i)
	{
		delete m_cMinimap->GetEnemyList()[i];
	}

	for(int i = 0; i < ParticleList.size(); ++i)
	{
		delete ParticleList[i];
	}

	delete m_cMinimap;

	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
