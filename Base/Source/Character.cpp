#include "Character.h"


CCharacter::CCharacter(void)
{
}


CCharacter::~CCharacter(void)
{
}

void CCharacter::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->setTranslate(pos);
	this->setScale(Vector3(1,1,1));
	Mtx44 R;
	R.SetToIdentity();
	this->setRotate(R);
	this->m_CurrentWeapon = NULL;
	this->health = 100;

	m_camera.Init(pos, target, up);
}

void CCharacter::AddWeapon(CWeapon &newWeapon)
{
	CWeapon *temp = new CWeapon(newWeapon);

	this->WeaponList.push_back(temp);
	if(this->m_CurrentWeapon == NULL)
	{
		this->m_CurrentWeapon = WeaponList[0];
	}
}

CWeapon* CCharacter::GetCurrentWeapon()
{
	return m_CurrentWeapon;
}

void CCharacter::ChangeWeapon(int i)
{
	m_CurrentWeapon = WeaponList[i];
}

int CCharacter::GetHealth()
{
	return health;
}

void CCharacter::SetHealth(int newHealth)
{
	this->health = newHealth;
}

void CCharacter::ReduceHealth()
{
	--health;
}

Camera3 CCharacter::GetCamera()
{
	return m_camera;
}
	
void CCharacter::UpdateCamera(double dt,std::vector<unsigned char> &heightMap, Vector3 terrainsize)
{
	m_camera.Update(dt, heightMap, terrainsize);
}

void CCharacter::UpdateCameraStatus(const unsigned char key)
{
	m_camera.UpdateStatus(key);
}