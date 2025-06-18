#pragma once

#include "Shape.h"

//Richie Nguyen, CS134-01 Midterm Project

class ExplosionParticle : public Shape {
public:

	ExplosionParticle();

	float maxLifespan;
	vector<glm::vec3> verts;
	bool isActive;

	void draw();
	void updateParticle();

};

