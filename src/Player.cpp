#include "Player.h"

//Richie Nguyen, CS134-01 Midterm Project

Player::Player() {
	projectiles.clear();
	verts.push_back(glm::vec3(-20.0, 20.0, 0));
	verts.push_back(glm::vec3(20.0, 20.0, 0));
	verts.push_back(glm::vec3(0.0, -30.0, 0));
	position = glm::vec3(5.0, 5.0, 0);
	radius = 16.0;
	rotVelLimit = 3.5;
	color = ofColor::limeGreen;
	rotationAccel = 0.5;
	name = "Player";
	isTangible = true;
	soundThrust.load("sounds/shipThrust.mp3");
	soundThrust.setVolume(0.6);
	soundThrust.setMultiPlay(true);
	soundThrust.setLoop(true);
	soundPlayerShoot.load("sounds/shipShoot.mp3");
	soundPlayerShoot.setVolume(0.1);
	soundPlayerShoot.setMultiPlay(true);
}

void Player::draw() {
	ofSetColor(color);
	ofPushMatrix();
	ofMultMatrix(getTransform());
	ofDrawTriangle(verts[0], verts[1], verts[2]);
	ofPopMatrix();
}

void Player::moveForward() {
	forces += heading() * 750.0;
	float velCheck = velocity.length();
	if (velCheck == 0) {
		velocity += heading() * 0.0001;
	}
	while (!soundThrust.isPlaying()) {
		soundThrust.play();
	}
}

void Player::moveBackward() {
	forces += -(heading()) * 750.0;
	float velCheck = velocity.length();
	if (velCheck == 0) {
		velocity += -(heading()) * 0.0001;
	}
	while (!soundThrust.isPlaying()) {
		soundThrust.play();
	}
}

void Player::checkCollision(glm::vec3 otherPosition, float otherRadius, bool otherTangible) {
	bool hasCollided = intersect(otherPosition, otherRadius, otherTangible);
	if (hasCollided == true) {
		this->isAlive = false;
		cout << "player died" << endl;
	}
}

void Player::checkCollision(ProjectileParticle inputProjectile) {
	bool hasCollided = intersect(inputProjectile.position, inputProjectile.radius, inputProjectile.isTangible);
	if (hasCollided == true) {
		this->isAlive = false;
		inputProjectile.isAlive = false;
		cout << "player died" << endl;
	}
}

void Player::beOpaque() {//making the shape opaque and not transparent
	if (color.a < 250.0) {
		color.a += 5.0;
	}

	if ((color.a >= 250.0) && (isTangible != true)) {
		isTangible = true;
	}
}

void Player::shootProjectiles() {

	ProjectileParticle *projectile = new ProjectileParticle();
	projectile->position = this->position;
	projectile->owner = name;
	projectile->angle = this->angle;
	projectile->velocity = (projectile->heading() * 1500) + this->velocity;

	projectiles.push_back(*projectile);
	soundPlayerShoot.play();

}

void Player::updateProjectiles() {

	if (!projectiles.empty()) {

		vector<ProjectileParticle> tempProjectiles;
		
		for (int i = 0; i < projectiles.size(); i++) {
			if ((projectiles[i].isAlive != false) || (projectiles[i].isAlive > 0)) {
				tempProjectiles.push_back(projectiles[i]);
			}
		}
		projectiles = tempProjectiles;

		for (int i = 0; i < projectiles.size(); i++) {
			projectiles[i].wrapPosition();
			projectiles[i].integrate();
		}
	}

}

void Player::drawProjectiles() {

	if (!projectiles.empty()) {
		for (int i = 0; i < projectiles.size(); i++) {
			projectiles[i].draw();
		}
	}

}