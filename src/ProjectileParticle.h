#pragma once

#include "Shape.h"

//Richie Nguyen, CS134-01 Midterm Project


class ProjectileParticle : public Shape {
public:

	ProjectileParticle();

	string owner;
	vector<glm::vec3> verts;

	void draw();
	void wrapPosition();
	

};

