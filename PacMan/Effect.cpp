#include "Effect.h"

Effect::Effect(int pointValue, state newState) {
	this->pointValue = pointValue;
	this->newState = newState;
}

int Effect::getPointValue() {
	return pointValue;
}

state Effect::getNewState() {
	return newState;
}