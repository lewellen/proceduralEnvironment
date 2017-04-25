#include <cstdio>

#include "IPainter.h"
#include "Renderable.h"

Renderable::Renderable() {
	painter = NULL;
	pass = RP_FINAL;
}

Renderable::~Renderable() {
	painter = NULL;
}

void Renderable::DoIdle() {

}

IPainter* Renderable::GetPainter() {
	return painter;
}

RenderPass Renderable::GetPass() {
	return pass;
}

void Renderable::SetPainter(IPainter* painter) {
	this->painter = painter;
}

void Renderable::SetPass(RenderPass pass) {
	this->pass = pass;
}