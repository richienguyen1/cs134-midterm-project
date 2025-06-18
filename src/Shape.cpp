#pragma once
#include "Shape.h"

//Richie Nguyen, CS134-01 Midterm Project

//  Base class for any object that needs a transform.
//
Shape::Shape() {
	position = glm::vec3(0.0, 0.0, 0.0);
	scale.set(1.0, 1.0, 1.0);
	angle = 0.0;
	rotVel = 0.0;
	rotVelLimit = 5.0;
	velocity.set(0.0, 0.0, 0.0);
	acceleration.set(0.0, 0.0, 0.0);
	forces.set(0.0, 0.0, 0.0);
	damping = 0.99;
	dampingRot = 0.95;
	mass = 1.0;
	radius = 4.0;
	color = ofColor::lightSteelBlue;//initialize shape
	rotationAccel = 0.1;
	name = "placeholder";
	lifespan = 0;
	isAlive = true;
	isTangible = false;
}

bool Shape::intersect(glm::vec3 otherPosition, float otherRadius, bool otherTangible) {//radius collision detection given an external position and radius and tangible flag
	if ((this->isTangible == true) && (otherTangible == true)) {//only run if both objects are tangible
		float distance = glm::length(this->position - otherPosition);
		float sumRadius = this->radius + otherRadius;
		if (distance < sumRadius) {//collision if distance < r1 + r2
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
	
}

void Shape::draw() {//default draw function if not specified by subclasses
	ofPushMatrix();
	ofMultMatrix(getTransform());
	ofSetColor(color);
	ofDrawCircle(position, radius);
	ofPopMatrix();
}

glm::mat4 Shape::getTransform() {//getting matrix transforms
	glm::mat4 T = glm::translate(glm::mat4(1.0), glm::vec3(position));
	glm::mat4 R = glm::rotate(glm::mat4(1.0), glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 S = glm::scale(glm::mat4(1.0), glm::vec3(scale));
	return (T * R * S);
}

glm::vec3 Shape::heading() {//getting heading vector
	glm::mat4 R = glm::rotate(glm::mat4(1.0), glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
	return glm::normalize(R * glm::vec4(glm::vec3(0.0, -1.0, 0.0), 1.0));
}

void Shape::integrate() {//physics movement with integration
	float dt = (1.0 / ofGetFrameRate());
	//1. update position with p' = p + vt
	position = position + (velocity * dt);
	//2. update acceleration from forces with f = ma -> a = f/m and update velocity with v' = v + at
	ofVec3f tempAccel = acceleration;
	tempAccel += (forces / mass);
	velocity = velocity + (tempAccel * dt);
	//3. dampen new velocity
	velocity *= damping;

	angle += rotVel;
	rotVel *= dampingRot;//also applying rotational acceleration/forces with integration

	forces.set(0.0, 0.0, 0.0);
}

void Shape::rotateLeft() {
	if (rotVel >= -rotVelLimit) {
		rotVel -= rotationAccel;
	}
	
}

void Shape::rotateRight() {
	if (rotVel <= rotVelLimit) {
		rotVel += rotationAccel;
	}
	
}

void Shape::wrapPosition() {//wrapping the shape to opposide window sides when it goes outside the window
	if (position.x > (ofGetWindowWidth() + (radius / 2.0))) {//right
		isTangible = false;
		color.a = 200.0;
		position = glm::vec3((radius / 2.0), position.y, position.z);
		
	}
	else if (position.x < -(radius / 2.0)) {//left
		isTangible = false;
		color.a = 200.0;
		position = glm::vec3(ofGetWindowWidth() - (radius / 2.0), position.y, position.z);
	}

	if (position.y > (ofGetWindowHeight() + (radius / 2.0))) {//down
		isTangible = false;
		color.a = 200.0;
		position = glm::vec3(position.x, (radius / 2.0), position.z);
	}
	else if (position.y < -(radius / 2.0)) {//up
		isTangible = false;
		color.a = 200.0;
		position = glm::vec3(position.x, ofGetWindowHeight() - (radius / 2.0), position.z);
	}
}

void Shape::beOpaque() {//making the shape opaque and not transparent
	if (color.a < 250.0) {
		color.a += 5.0;
	}
}

