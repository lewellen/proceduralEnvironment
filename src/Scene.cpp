#include <cstdio>
#include <cmath>
#include <ctime>
#include <vector>
#include <algorithm>

#include "OpenGL.h"

#include "Scene.h"

#include "Butterfly.h"
#include "Forest.h"
#include "House.h"
#include "Neighborhood.h"
#include "RenderableCollection.h"
#include "Terrain.h"
#include "Water.h"

Scene::Scene() : RenderableCollection() {
	Terrain* terrain = new Terrain(7);
	water = new Water(0, terrain);
	neighborhood = new Neighborhood(1, terrain);

	RenderableCollection::Items()->push_back(terrain);
	RenderableCollection::Items()->push_back(water);
	RenderableCollection::Items()->push_back(new Forest(4, terrain));
	RenderableCollection::Items()->push_back(neighborhood);

	for (int i = 0; i < 6; i++) {
		Butterfly* butterfly = new Butterfly();
		RenderableCollection::Items()->push_back(butterfly);
	}
}

Scene::~Scene() {

}

Water* Scene::GetWater() {
	return water;
}

void Scene::ToggleSmoke() {
	neighborhood->ToggleSmoke();
}


//
//void Scene::DoIdle() {
//	//plume->DoTimeStep();
//
//	//// Gyroid
//	//for (int row = 0; row < edge; row++) {
//	//	for (int column = 0; column < edge; column++) {
//	//		for (int plane = 0; plane < edge; plane++) {
//	//			float tau = 3.1415926535897932 * 2.0;
//	//			float x = tau * column / edge;
//	//			float y = tau * row / edge;
//	//			float z = tau * plane / edge;
//	//			data[row][column][plane] = sinf(x) * cosf(y) + sinf(y) * cosf(z) + sinf(z) * cosf(x);
//
//	//		}
//	//	}
//	//}
//}
