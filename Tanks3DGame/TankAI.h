#pragma once
#include "Tank.h"



class TankAI : public Tank
{
public:
	TankAI(const Model* model, const Texture* texture);
	~TankAI();

	void Update(double deltaTime);
};

