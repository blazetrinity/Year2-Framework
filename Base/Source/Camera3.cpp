#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

static const float CAMERA_SPEED = 200.f;

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	stance = PLAYER_STANCE::STANCE_STAND;

	for(int i = 0; i < 255; ++i)
	{
		myKeys[i] = false;
	}
}

void Camera3::Update(double dt,std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	if(myKeys['a'] == true)
	{
		MoveLeft(dt,heightMap,terrainSize);
		myKeys['a'] = false;
	}
	if(myKeys['d'] == true)
	{
		MoveRight(dt,heightMap,terrainSize);
		myKeys['d'] = false;
	}
	if(myKeys['w'] == true)
	{
		MoveForward(dt,heightMap,terrainSize);
		myKeys['w'] = false;
	}
	if(myKeys['s'] == true)
	{
		MoveBackward(dt,heightMap,terrainSize);
		myKeys['s'] = false;
	}
	if(Application::IsKeyPressed(VK_LEFT))
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(CAMERA_SPEED * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
	}
	if(Application::IsKeyPressed(VK_RIGHT))
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(-CAMERA_SPEED * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
	}
	if(Application::IsKeyPressed(VK_UP))
	{
		float pitch = (float)(CAMERA_SPEED * (float)dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	if(Application::IsKeyPressed(VK_DOWN))
	{
		float pitch = (float)(-CAMERA_SPEED * (float)dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}

	//Update the camera direction based on mouse move
	//left-right rotate
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(-CAMERA_SPEED * Application::camera_yaw * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw,0,1,0);
		view = rotation * view;
		target = position + view;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
	}

	{
		float pitch = (float)(-CAMERA_SPEED * Application::camera_pitch * (float)dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	
	if(Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void Camera3::UpdateStatus(const unsigned char key)
{
	myKeys[key] = true;
}

void Camera3::MoveForward(const double dt, std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	Vector3 view = (target - position).Normalized();
	position += view * CAMERA_SPEED * (float)dt;
	target += view * CAMERA_SPEED * (float)dt;
	float yDiff = target.y - position.y;
	position.y = 10 + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
	target.y = position.y + yDiff;
}
	
void Camera3::MoveBackward(const double dt, std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	Vector3 view = (target - position).Normalized();
	position -= view * CAMERA_SPEED * (float)dt;
	target -= view * CAMERA_SPEED * (float)dt;
	float yDiff = target.y - position.y;
	position.y = 10 + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
	target.y = position.y + yDiff;
}	

void Camera3::MoveLeft(const double dt, std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	position -= right * CAMERA_SPEED * (float)dt;
	target -= right * CAMERA_SPEED * (float)dt;
	float yDiff = target.y - position.y;
	position.y = 10 + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
	target.y = position.y + yDiff;
}

void Camera3::MoveRight(const double dt, std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	position += right * CAMERA_SPEED * (float)dt;
	target += right * CAMERA_SPEED * (float)dt;
	float yDiff = target.y - position.y;
	position.y = 10 + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
	target.y = position.y + yDiff;
}

void Camera3::ChangeStance(std::vector<unsigned char> &heightMap, Vector3 terrainSize)
{
	if(stance == PLAYER_STANCE::STANCE_STAND)
	{
		stance = PLAYER_STANCE::STANCE_CROUCH;
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position.y = 10 + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
		up = right.Cross(view).Normalized();
		target = position + view;
	}

	else if(stance == PLAYER_STANCE::STANCE_CROUCH)
	{
		stance = PLAYER_STANCE::STANCE_PRONE;
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position.y = 7 + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
		up = right.Cross(view).Normalized();
		target = position + view;
	}

	else if(stance == PLAYER_STANCE::STANCE_PRONE)
	{
		stance = PLAYER_STANCE::STANCE_STAND;
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position.y = 5 + terrainSize.y * ReadHeightMap(heightMap, position.x/terrainSize.x, position.z/terrainSize.z);
		up = right.Cross(view).Normalized();
		target = position + view;
	}
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}