#include "ProjectileParticle.h"

//Richie Nguyen, CS134-01 Midterm Project

ProjectileParticle::ProjectileParticle() {

	owner = "placeholder";
	position = glm::vec3(0.0, 0.0, 0.0);
	angle = 0.0;
	radius = 10.0;
	color = ofColor::steelBlue;
	isAlive = true;
	isTangible = true;
	damping = 0.99975;
	rotVel = 0.0;
	rotVelLimit = 0.0;
	rotationAccel = 0.0;

	verts.push_back(glm::vec3(-7.5, 10, 0.0));
	verts.push_back(glm::vec3(0, -15, 0.0));
	verts.push_back(glm::vec3(7.5, 10, 0.0));

}

void ProjectileParticle::draw() {

	ofPushMatrix();
	ofMultMatrix(getTransform());
	ofSetColor(color);
	ofDrawTriangle(verts[0], verts[1], verts[2]);
	ofPopMatrix();

}

void ProjectileParticle::wrapPosition() {//overriding base wrapPosition function to make the projectile expire instead of wrapping around the screen

	if (position.x > (ofGetWindowWidth() + (radius / 2.0))) {//right
		isTangible = false;
		color.a = 15.0;
		isAlive = false;

	}
	else if (position.x < -(radius / 2.0)) {//left
		isTangible = false;
		color.a = 15.0;
		isAlive = false;
	}

	if (position.y > (ofGetWindowHeight() + (radius / 2.0))) {//down
		isTangible = false;
		color.a = 15.0;
		isAlive = false;
	}
	else if (position.y < -(radius / 2.0)) {//up
		isTangible = false;
		color.a = 15.0;
		isAlive = false;
	}

}
