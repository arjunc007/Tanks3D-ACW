#pragma once
#include <string>
#include "Message.h"
#include "DirectXMath.h"

class ThrustMessage
	: public Message
{
	// Data
protected:
	DirectX::XMFLOAT3 _thrust;

	// Structors
public:
	ThrustMessage(DirectX::XMFLOAT3 thrustAmt);
	virtual ~ThrustMessage();


	// Gets/Sets
public:
	const DirectX::XMFLOAT3& GetThrust()		const { return _thrust; }



	// Functions
public:


};