#pragma once
#include "NetworkComponent.h"

class TankNetworkComponent :
	public NetworkComponent
{
public:
	TankNetworkComponent(GameObject* gob);
	~TankNetworkComponent();
};

