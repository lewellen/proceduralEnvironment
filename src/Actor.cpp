#include <cstdio>

#include "Actor.h"

#include "IFrame.h"

Actor::Actor(IFrame* frame) {
	this->frame = frame;
}

Actor::~Actor() {
	// IFrame is external value so it is not actor's responsibility to destroy.
	this->frame = NULL;
}

IFrame* Actor::GetFrame() {
	return this->frame;
}

void Actor::SetFrame(IFrame* frame) {
	this->frame = frame;
}