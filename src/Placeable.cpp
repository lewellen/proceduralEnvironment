#include "Placeable.h"

Placeable::Placeable() {
	Set(0, 0);
	SetScale(1.0);
}

int Placeable::GetColumn() {
	return this->column;
}

int Placeable::GetRow() {
	return this->row;
}

float Placeable::GetScale() {
	return this->scale;
}

void Placeable::Set(int row, int column) {
	this->row = row;
	this->column = column;
}

void Placeable::SetScale(float value) {
	this->scale = value;
}