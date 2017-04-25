#include <cstdio>

#include "WaterShader.h"
#include "Shader.h"

WaterShader::WaterShader() : Shader("shaders/water.vert", "shaders/water.frag") {
	time = uniform("time");
	minSeaFloor = uniform("minSeaFloor");

	refleTex = uniform("reflectionTexture");
	refraTex = uniform("refractionTexture");
	bumpTex = uniform("bumpTexture");
	depthTex = uniform("depthTexture");

	seaFloor = attribute("seaFloor");
	applyWave = attribute("applyWave");
}

WaterShader::~WaterShader() {

}

void WaterShader::SetTime(float time) {
	setUniformf(this->time, time);
}

void WaterShader::SetMinSeaFloor(float minSeaFloor) {
	setUniformf(this->minSeaFloor, minSeaFloor);
}

void WaterShader::SetSeaFloor(float seaFloor) {
	setAttributef(this->seaFloor, seaFloor);
}

void WaterShader::SetApplyWave(float applyWave) {
	setAttributef(this->applyWave, applyWave);
}

void WaterShader::SetBumpTexture(int i) {
	setUniformi(this->bumpTex, i);
}

void WaterShader::SetDepthTexture(int i) {
	setUniformi(this->depthTex, i);
}

void WaterShader::SetReflectionTexture(int i) {
	setUniformi(this->refleTex, i);
}

void WaterShader::SetRefractionTexture(int i) {
	setUniformi(this->refraTex, i);
}