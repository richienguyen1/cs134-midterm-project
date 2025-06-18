#pragma once
#include "ofMain.h"

//Richie Nguyen, CS134-01 Midterm Project

//base shape class for players/entities

class Shape {
public:
	Shape();

	glm::vec3 position;
	ofVec3f scale;
	float angle;//degrees
	float rotVel;
	float rotVelLimit;
	ofVec3f velocity;
	ofVec3f acceleration;
	ofVec3f forces;
	float damping;
	float dampingRot;
	float mass;//veriables for physics/movement
	string name;
	float lifespan;
	bool isTangible = false;//used for collisions
	bool isAlive = true;
	ofColor color;
	float radius;//collision purposes

	bool intersect(glm::vec3 otherPosition, float otherRadius, bool otherTangible);
	virtual void draw();
	virtual void beOpaque();
	glm::mat4 getTransform();
	glm::vec3 heading();
	void integrate();
	float rotationAccel;
	virtual void rotateLeft();
	virtual void rotateRight();
	virtual void wrapPosition();
	


};