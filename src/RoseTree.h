#ifndef __RoseTree_H__
#define __RoseTree_H__

#include<vector>

#include "RoseTree.h"

// An n-ary tree annotated with information useful for generating plants.
class RoseTree {
private:
	float baseRadius, length;
	int theta, phi;
	int depth;

public:
	// Initalizes a new instance. BaseRadius is the radius of the base of the 
	// branch, length is the length of the branch, theta is the yaw, phi the 
	// pitch and depth the depth of the node in the tree.
	RoseTree(float baseRadius, float length, int theta, int phi, int depth);

	// Releases memory associated with the instance.
	~RoseTree();

	// Children of the instance.
	std::vector<RoseTree*> Children;

	// Gets the base radius.
	float GetBaseRadius();

	// Gets the length.
	float GetLength();

	// Gets the depth of the node in the tree.
	int GetDepth();

	// Gets the pitch of the branch in degrees.
	int GetPhi();

	// Gets the yaw of the branch in degrees.
	int GetTheta();
};

#endif
