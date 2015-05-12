#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include "LoadHmap.h"

class Camera3 : public Camera
{
public:
	enum PLAYER_STANCE
	{
		STANCE_STAND = 0,
		STANCE_CROUCH,
		STANCE_PRONE,
	};

	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	bool myKeys[255];

	PLAYER_STANCE stance;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt, std::vector<unsigned char> &heightMap,Vector3 terrainSize);
	virtual void Reset();

	void ChangeStance(std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	void UpdateStatus(const unsigned char key);

	virtual void MoveForward(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void MoveBackward(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void MoveLeft(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
	virtual void MoveRight(const double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize);
};

#endif