#pragma once

#include "ofMain.h"
#include "Shape.h"
#include "ProjectileParticle.h"

//Richie Nguyen, CS134-01 Midterm Project

class Entity : public Shape {
public:

	Entity();
	Entity(glm::vec3 importPosition, float importRadius, ofColor importColor, float importMass, int importMovement, int importChampion, vector<glm::vec3> importVerts);
	glm::mat4 getTransformT();
	glm::mat4 getTransformR(float visAngle);
	glm::mat4 getTransformS();
	void integrate();
	void draw();
	void move();
	void beOpaque();
	bool checkCollision(ProjectileParticle inputParticle);
	void shootProjectiles();
	void updateProjectiles();
	void drawProjectiles();
	
	int rotDir = 0;
	float visAngle;
	vector<glm::vec3> verts;
	int movementType;
	int championType;
	int collidedCount;
	float timeSinceCollision = 0.0;
	bool hasParent;
	float shootTimer = 0.0;
	float timeUntilShoot = 1.5;
	vector<ProjectileParticle> projectiles;
	vector<ProjectileParticle> preloadedProjectiles;
	ofSoundPlayer soundEntityShoot;//sound modified from freesound.org/people/Defunct3/sounds/77172/

};

