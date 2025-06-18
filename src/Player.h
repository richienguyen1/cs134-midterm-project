#pragma once

#include "Shape.h"
#include "ProjectileParticle.h"

//Richie Nguyen, CS134-01 Midterm Project

class Player : public Shape {
public: 

	Player();
	void draw();

	void moveForward();
	void moveBackward();
	void checkCollision(glm::vec3 otherPosition, float otherRadius, bool otherTangible);
	void checkCollision(ProjectileParticle inputParticle);
	void beOpaque();
	void shootProjectiles();
	void updateProjectiles();
	void drawProjectiles();

	vector<glm::vec3> verts;
	vector<ProjectileParticle> projectiles;
	ofSoundPlayer soundThrust;//sounds modified from freesound.org/people/OrbitalChiller/sounds/432712/ and freesound.org/people/portwain/sounds/261129/
	ofSoundPlayer soundPlayerShoot;//sound modified from freesound.org/people/mohawkade/sounds/351307/
};

