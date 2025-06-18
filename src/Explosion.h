#pragma once

#include "ExplosionParticle.h"

//Richie Nguyen, CS134-01 Midterm Project

class Explosion
{
public:

	Explosion();
	float minSpeed;
	float maxSpeed;
	int totalParticles;
	bool hasFired;
	bool firedOnce;
	vector<ExplosionParticle*> explosionParticles;
	glm::vec3 position;
	ofSoundPlayer soundPlayerExplode;//sounds modified from freesound.org/people/TB0Y298/sounds/718960/ and freesound.org/people/ABloemarts242029/sounds/765135/

	void explodeParticles();
	void drawParticles();
	void updateParticles();
	void resetParticles();

};

