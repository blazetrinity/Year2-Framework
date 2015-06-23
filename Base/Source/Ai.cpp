#include "Ai.h"


CAi::CAi(void) : m_Speed(100), m_state(false)
{
}


CAi::~CAi(void)
{
}

void CAi::Update(Vector3 PlayerPos, double dt, Vector3 terrainSize, std::vector<unsigned char> *heightMap)
{
	vel = (PlayerPos - this->getTranslate()).Normalized();
	vel = m_Speed * vel;
	Vector3 newPos = this->getTranslate() + vel * dt;

	this->setTranslate(Vector3 (newPos.x, 10 + terrainSize.y * ReadHeightMap(*heightMap, newPos.x/terrainSize.x, newPos.z/terrainSize.z), newPos.z));
	this->setBound(Vector3(2,1,2));
}

void CAi::setState(bool newstate)
{
	this->m_state = newstate;
}

bool CAi::getState()
{
	return m_state;
}
