#ifndef CHARACTER_H
#define CHARACTER_H

#include "Camera3.h"
#include "ObjectClass.h"
#include "Weapon.h"
#include <vector>

class CCharacter : public CObjectClass
{
public:
	CCharacter(void);
	~CCharacter(void);

	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void AddWeapon(CWeapon& newWeapon);
	void ChangeWeapon(int i);
	CWeapon* GetCurrentWeapon();
	int GetHealth();
	void SetHealth(int newhealth);
	void ReduceHealth();
	Camera3 GetCamera();
	void UpdateCamera(double dt, std::vector<unsigned char> &heightMap, Vector3 terrainsize);
	void UpdateCameraStatus(const unsigned char key);

private:
	std::vector<CWeapon*> WeaponList;
	Camera3 m_camera;
	CWeapon* m_CurrentWeapon;
	int health;
};

#endif