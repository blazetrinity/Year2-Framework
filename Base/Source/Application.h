
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "SceneText.h"
#include "GenerateSkyPlane.h"

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	static double camera_yaw, camera_pitch;
	bool GetMouseUpdate();
	bool GetKeyboardUpdate();
	const static int m_window_width = 800;
	const static int m_window_height = 600;

private:
	Application();
	~Application();

	const static int m_window_deadzone = 100;

	//Declare a window object
	StopWatch m_timer;
	double m_dElapsedTime;
	double m_dAccumulatedTime_ThreadOne;
	double m_dAccumulatedTime_ThreadTwo;
	
	//The handler for the scene
	Scene *scene;
};

#endif