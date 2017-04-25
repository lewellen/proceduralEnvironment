#include <cstdio>

#include "OpenGL.h"

#include<vector>

#include "RenderableCollection.h"

#include "Renderable.h"

RenderableCollection::RenderableCollection() {
	this->items = new std::vector<Renderable*>();
}

RenderableCollection::~RenderableCollection() {
	FreeCollection();

	delete items;
	items = NULL;
}

void RenderableCollection::FreeCollection() {
	std::vector<Renderable*>::iterator i = this->items->begin();
	std::vector<Renderable*>::iterator end = this->items->end();
	for (; i != end; i++)
		delete (*i);

	items->clear();
}

std::vector<Renderable*>* RenderableCollection::Items() {
	return this->items;
}

void RenderableCollection::DoIdle() {
	std::vector<Renderable*>::iterator i = this->items->begin();
	std::vector<Renderable*>::iterator end = this->items->end();
	for (; i != end; i++)
		(*i)->DoIdle();
}

void RenderableCollection::Render() {
	std::vector<Renderable*>::iterator i = this->items->begin();
	std::vector<Renderable*>::iterator end = this->items->end();
	for (; i != end; i++) {
		glPushMatrix();
		doGlTransforms(*i);
		(*i)->Render();
		glPopMatrix();
	}
}

void RenderableCollection::SetPass(RenderPass pass) {
	Renderable::SetPass(pass);

	std::vector<Renderable*>::iterator i = this->items->begin();
	std::vector<Renderable*>::iterator end = this->items->end();
	for (; i != end; i++)
		(*i)->SetPass(pass);
}

void RenderableCollection::doGlTransforms(Renderable* renderable) {

}