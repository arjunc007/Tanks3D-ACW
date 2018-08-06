#include "TankAI.h"



TankAI::TankAI(const Model* model, const Texture* texture) : Tank(model, texture)
{

	//Get a random XZ location
	float step = 10.0f;
	int sign = rand() % 100 + 1;
	int x = step;
	if(sign > 50)
		x += step * (rand() % 9);
	else
		x -= step * (rand() % 9);

	sign = rand() % 100 + 1;
	int z = step;
	if (sign > 50)
		z += step * (rand() % 9);
	else
		z -= step * (rand() % 9);

	m_position.x = x;
	m_position.z = z;
	
	
}


TankAI::~TankAI()
{
}

void TankAI::Update(double deltaTime)
{

}