#include "Explosion.h"

//Richie Nguyen, CS134-01 Midterm Project

Explosion::Explosion() {//basic particle explosion system

	minSpeed = 500.0;
	maxSpeed = 1500.0;
	totalParticles = 100;
	position = glm::vec3(0.0, 0.0, 0.0);
	explosionParticles.clear();
	hasFired = false;
	firedOnce = false;
	soundPlayerExplode.load("sounds/shipExplode.mp3");
	soundPlayerExplode.setVolume(0.75);

}

void Explosion::explodeParticles() {

	if (hasFired != true) {//if explosion has not been fired

		if (firedOnce != true) {//if explosion has not activated once
			firedOnce = true;
			for (int i = 0; i < totalParticles; i++) {//creating particles for explosion
				ExplosionParticle *particle = new ExplosionParticle();
				particle->position = this->position;
				particle->color = ofColor::green;
				particle->velocity = particle->heading() * ofRandom(minSpeed, maxSpeed);//randomizing speed of particle
				
				explosionParticles.push_back(particle);
			}
		}

		hasFired = true;
		
		soundPlayerExplode.play();
	}

}

void Explosion::drawParticles() {

	if (!explosionParticles.empty()) {
		for (int i = 0; i < explosionParticles.size(); i++) {
			explosionParticles[i]->draw();
		}
	}

}

void Explosion::updateParticles() {

	if (!explosionParticles.empty()) {
		for (int i = 0; i < explosionParticles.size(); i++) {
			explosionParticles[i]->updateParticle();//updating lifespan
			explosionParticles[i]->integrate();//particle movement
		}

		vector<ExplosionParticle*> tempList;
		for (int i = 0; i < explosionParticles.size(); i++) {
			if (explosionParticles[i]->isActive != false) {
				tempList.push_back(explosionParticles[i]);
			}
		}
		explosionParticles = tempList;
	}
}

void Explosion::resetParticles() {//empty particle array
	explosionParticles.clear();
}