#include "ExplosionParticle.h"

//Richie Nguyen, CS134-01 Midterm Project

ExplosionParticle::ExplosionParticle() {

	position = glm::vec3(0.0, 0.0, 0.0);
	angle = ofRandom(0.0, 360.0);
	mass = 0.5;
	color = ofColor::orangeRed;
	damping = ofRandom(0.97, 0.995);
	radius = ofRandom(5.0, 15.0);
	isAlive = false;
	isActive = true;
	maxLifespan = ofRandom(2.0, 3.0);

	verts.push_back(glm::vec3(-radius, radius / 2.0, 0.0) * ofNoise(angle));
	verts.push_back(glm::vec3(0.0, -radius / 2.0, 0.0) * ofNoise(damping));
	verts.push_back(glm::vec3(radius, radius / 2.0, 0.0) * ofNoise(radius));

}

void ExplosionParticle::draw() {
	ofPushMatrix();
	ofMultMatrix(getTransform());
	ofSetColor(color);
	ofDrawTriangle(verts[0], verts[1], verts[2]);
	ofPopMatrix();
}

void ExplosionParticle::updateParticle() {
	if (lifespan >= maxLifespan) {
		isActive = false;
	}
	else {
		lifespan += (1.0 / ofGetFrameRate());
	}
}