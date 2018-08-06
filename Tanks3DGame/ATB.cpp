#include "ATB.h"
#include "Components.h"
#include "Window.h"

ATB::ATB()
	: GameObject("ATB")
{
	new ATBComponent(this);
}


ATB::~ATB()
{
}

void ATB::AddDoubleVar(const char* name, double* var, const char* desc)
{
	ATBComponent* atbc = (ATBComponent*)GetComponent("ATB");
	TwAddVarRW(atbc->GetBar(), name, TW_TYPE_DOUBLE, var, desc);
}

void ATB::AddIntVar(const char* name, int* var, const char* desc)
{
	ATBComponent* atbc = (ATBComponent*)GetComponent("ATB");
	TwAddVarRW(atbc->GetBar(), name, TW_TYPE_INT32, var, desc);
}

void ATB::Draw()
{
	ATBComponent* atbc = (ATBComponent*)GetComponent("ATB");
	atbc->Draw();
}
