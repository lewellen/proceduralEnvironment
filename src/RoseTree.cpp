#include <cstdio>
#include <vector>

#include "RoseTree.h"

RoseTree::RoseTree(float baseRadius, float length, int theta, int phi, int depth) {
	this->baseRadius = baseRadius;
	this->length = length;
	this->theta = theta;
	this->phi = phi;
	this->depth = depth;

	Children = std::vector<RoseTree*>();
}

RoseTree::~RoseTree() {
	std::vector<RoseTree*>::iterator i = this->Children.begin();
	std::vector<RoseTree*>::iterator end = this->Children.end();
	for (; i != end; i++)
		delete *i;
}

float RoseTree::GetBaseRadius() {
	return this->baseRadius;
}

float RoseTree::GetLength() {
	return this->length;
}

int RoseTree::GetTheta() {
	return this->theta;
}

int RoseTree::GetPhi() {
	return this->phi;
}

int RoseTree::GetDepth(){
	return this->depth;
}