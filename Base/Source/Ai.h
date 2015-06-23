#ifndef AI_H
#define AI_H

#include "ObjectClass.h"

class CAi : public CObjectClass
{
public:
	CAi(void);
	~CAi(void);

	void Update(Vector3 PlayerPos, double dt, Vector3 terrainSize, std::vector<unsigned char> *heightMap);
	bool getState();
	void setState(bool newstate);
	

private:
	Vector3 vel;
	float m_Speed;
	bool m_state;
};

#endif

