#pragma once
#include "MyEnums.h"
class Effect{
	public:
		Effect(int pointValue, state newState);
		int getPointValue();
		state getNewState();
	private:
		int pointValue;
		state newState;

};

