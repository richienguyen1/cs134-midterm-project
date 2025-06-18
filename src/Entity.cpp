
#include "Entity.h"

//Richie Nguyen, CS134-01 Midterm Project

/// <list of movement types>
/// 0: normal; move towards the heading vector
/// 1: random; randomly move around regardless of heading vector, may use noise values
/// </list of movement types>

/// <list of entity champion types similar to risk of rain elite enemies or binding of isaac champion enemies>
/// 0: normal; no changes
/// 1: large split; splits into many smaller entities on death instead of around 3-5
/// 2: cross shot; fires four projectiles in a cross (+) pattern
/// 3: x shot; fires four projectiles in an x pattern
/// 4: radial spread; fires a bunch of projectiles at once radially
/// 5: ghost; is not affected by enitity collision checks
/// 6: everything; includes above champion types, really rare
/// </list of entity champion types>

Entity::Entity() {
	verts.clear();
	rotDir = glm::round(ofRandom(0, 1));
	position = glm::vec3(ofGetWindowWidth()/2, ofGetWindowHeight()/2, 0.0);
	radius = 30.0;
	angle = ofRandom(359.0);
	visAngle = angle;
	rotVel = ofRandom(0.25, 0.75);
	rotVelLimit = ofRandom(2.5, 7.5);
	color = ofColor::lightGray;
	color.a = 255.0;
	mass = 2.0;
	name = "placeholder entity";
	isAlive = true;
	isTangible = false;
	movementType = 0;
	championType = 0;
	hasParent = false;
	collidedCount = 0;
	projectiles.clear();
	preloadedProjectiles.reserve(16);
	for (int i = 0; i < 16; i++) {
		ProjectileParticle *loadProj = new ProjectileParticle();
		preloadedProjectiles.push_back(*loadProj);
	}
	soundEntityShoot.load("sounds/entityShoot.mp3");
	soundEntityShoot.setVolume(0.4);
	soundEntityShoot.setMultiPlay(true);

	//rotationAccel = ofRandom(-5, 5);

	verts.push_back(ofVec3f(0, 50, 0));
	verts.push_back(ofVec3f(25, 35, 0));
	verts.push_back(ofVec3f(40, 17.5, 0));
	verts.push_back(ofVec3f(50, 0, 0));
	verts.push_back(ofVec3f(25, -30, 0));
	verts.push_back(ofVec3f(0, -50, 0));
	verts.push_back(ofVec3f(-27.5, -20, 0));
	verts.push_back(ofVec3f(-50, 0, 0));
	verts.push_back(ofVec3f(-40, 15, 0));
	verts.push_back(ofVec3f(-20, 35, 0));
}

Entity::Entity(glm::vec3 importPosition, float importRadius, ofColor importColor, float importMass, int importMovement, int importChampion, vector<glm::vec3> importVerts) {
	position = glm::vec3(importPosition);
	rotDir = glm::round(ofRandom(0, 1));
	radius = importRadius;
	angle = ofRandom(359.0);
	visAngle = angle;
	rotVel = ofRandom(0.25, 0.75);
	rotVelLimit = ofRandom(2.5, 5);
	color = importColor;
	mass = importMass;
	movementType = importMovement;
	championType = importChampion;
	name = "Spawned Entity";
	isAlive = true;
	isTangible = false;
	hasParent = false;
	collidedCount = 0;
	projectiles.clear();
	preloadedProjectiles.reserve(16);
	for (int i = 0; i < 16; i++) {
		ProjectileParticle* loadProj = new ProjectileParticle();
		preloadedProjectiles.push_back(*loadProj);
	}
	soundEntityShoot.load("sounds/entityShoot.mp3");
	soundEntityShoot.setVolume(0.4);
	soundEntityShoot.setMultiPlay(true);
	//rotationAccel = ofRandom(-5, 5);

	verts = importVerts;

}

void Entity::integrate() {//physics movement with integration; rotations are only done visually
	float dt = (1.0 / ofGetFrameRate());
	//1. update position with p' = p + vt
	position = position + (velocity * dt);
	//2. update acceleration from forces with f = ma -> a = f/m and update velocity with v' = v + at
	ofVec3f tempAccel = acceleration;
	tempAccel += (forces / mass);
	velocity = velocity + (tempAccel * dt);
	//3. dampen new velocity
	velocity *= damping;

	visAngle += rotVel;
	rotVel *= dampingRot;//also applying rotational acceleration/forces with integration

	forces.set(0.0, 0.0, 0.0);
}

glm::mat4 Entity::getTransformT() {//getting matrix transforms; translate
	glm::mat4 T = glm::translate(glm::mat4(1.0), glm::vec3(position));
	return (T);
}

glm::mat4 Entity::getTransformR(float visAngle) {//getting matrix transforms; rotate
	glm::mat4 R = glm::rotate(glm::mat4(1.0), glm::radians(visAngle), glm::vec3(0.0, 0.0, 1.0));
	return (R);
}

glm::mat4 Entity::getTransformS() {//getting matrix transforms; scale
	glm::mat4 S = glm::scale(glm::mat4(1.0), glm::vec3(scale));
	return (S);
}

void Entity::draw() {//drawing entity
	
	ofSetColor(color);
	ofPushMatrix();
	ofMultMatrix(getTransformT());
	ofMultMatrix(getTransformR(visAngle));
	ofMultMatrix(getTransformS());//separeted usual TRS matrix to separate matrices to account for visual rotations
	for (int i = 0; i < verts.size(); i++) {
		if (i == (verts.size() - 1)) {
			ofDrawLine(verts[i], verts[0]);
		}
		else {
			ofDrawLine(verts[i], verts[i + 1]);
		}
	}
	ofPopMatrix();
}

void Entity::move() {//moving entities
	float velCheck = velocity.length();
	switch (rotDir) {//rotation direction
	case(0)://left
		rotateLeft();
		break;
	case(1)://right
		rotateRight();
		break;
	}

	float initAngle = angle + (ofRandom(-30.0, 30.0));
	int randomChance = glm::round(ofRandom(100));
	switch (movementType) {//movement type based on generation
	case (0)://normal movement
		forces += heading() * ofRandom(250, 600);
		if (velCheck == 0) {
			velocity += heading() * 0.0001;
		}
		break;
	case (1)://random
		forces += heading() * ofRandom(300, 650);
		if (velCheck == 0) {
			velocity += heading() * 0.0001;
		}
		if (randomChance >= 82) {
			angle = ofRandom(0.0, 360.0);
			forces += heading() * ofRandom(800, 1250);
		}
		break;
	}
}

void Entity::beOpaque() {//making the shape opaque and not transparent
	if (color.a < 250.0) {
		color.a += 5.0;
	}
}

bool Entity::checkCollision(ProjectileParticle inputProjectile) {//checking collisions with player projectiles, bool to check for projectile lifespan
	bool projectileStatus = false;
	bool hasCollided = intersect(inputProjectile.position, inputProjectile.radius, inputProjectile.isTangible);
	if ((hasCollided == true) && (inputProjectile.owner == "Player")) {
		this->isAlive = false;
		projectileStatus = true;
	}
	return projectileStatus;
}

void Entity::shootProjectiles() {//accounting for champion types that shoot projectiles
	
	if (shootTimer >= timeUntilShoot) {

		shootTimer = 0;
		float initAngle = 0;
		float initAngle2 = 0;
		switch (championType) {
		case(2)://cross shot; shoot in + pattern, up to 4 projectiles
			initAngle = 0;
			for (int i = 0; i < 4; i++) {
				preloadedProjectiles[i].position = this->position;
				preloadedProjectiles[i].color = ofColor::orangeRed;
				preloadedProjectiles[i].owner = name;
				preloadedProjectiles[i].angle = initAngle;
				preloadedProjectiles[i].velocity = (preloadedProjectiles[i].heading() * 500.0);
				projectiles.push_back(preloadedProjectiles[i]);
				initAngle += 90;
			}
			soundEntityShoot.play();
			break;
		case(3)://x short; shoot in x pattern, up to 4 projectiles
			initAngle = 45;
			for (int i = 0; i < 4; i++) {
				preloadedProjectiles[i].position = this->position;
				preloadedProjectiles[i].color = ofColor::orange;
				preloadedProjectiles[i].owner = name;
				preloadedProjectiles[i].angle = initAngle;
				preloadedProjectiles[i].velocity = (preloadedProjectiles[i].heading() * 500.0);
				projectiles.push_back(preloadedProjectiles[i]);
				initAngle += 90;
			}
			soundEntityShoot.play();
			break;
		case(4)://radial spread; spray of rotation projectiles, up to 8 projectiles
			initAngle = 0;
			for (int i = 0; i < 8; i++) {
				preloadedProjectiles[i].position = this->position;
				preloadedProjectiles[i].color = ofColor::hotPink;
				preloadedProjectiles[i].owner = name;
				preloadedProjectiles[i].angle = initAngle;
				preloadedProjectiles[i].velocity = (preloadedProjectiles[i].heading() * ofRandom(400, 800));
				projectiles.push_back(preloadedProjectiles[i]);
				initAngle += ofRandom(50, 80);
			}
			soundEntityShoot.play();
			break;
		case(6)://everything; all bullet patterns, up to 16 projectiles
			initAngle = 0;
			for (int i = 0; i < 4; i++) {//section for + shot
				preloadedProjectiles[i].position = this->position;
				preloadedProjectiles[i].color = ofColor::mediumVioletRed;
				preloadedProjectiles[i].owner = name;
				preloadedProjectiles[i].angle = initAngle;
				preloadedProjectiles[i].velocity = (preloadedProjectiles[i].heading() * ofRandom(500.0, 750.0));
				projectiles.push_back(preloadedProjectiles[i]);
				initAngle += 90;
			}
			initAngle2 = 45;
			for (int i = 4; i < 8; i++) {//section for x shot
				preloadedProjectiles[i].position = this->position;
				preloadedProjectiles[i].color = ofColor::indianRed;
				preloadedProjectiles[i].owner = name;
				preloadedProjectiles[i].angle = initAngle2;
				preloadedProjectiles[i].velocity = (preloadedProjectiles[i].heading() * ofRandom(500.0, 750.0));
				projectiles.push_back(preloadedProjectiles[i]);
				initAngle2 += 90;
			}
			initAngle = 0;
			for (int i = 8; i < 16; i++) {//section for radial spread
				preloadedProjectiles[i].position = this->position;
				preloadedProjectiles[i].color = ofColor::fuchsia;
				preloadedProjectiles[i].owner = name;
				preloadedProjectiles[i].angle = initAngle;
				preloadedProjectiles[i].velocity = (preloadedProjectiles[i].heading() * ofRandom(550, 800));
				projectiles.push_back(preloadedProjectiles[i]);
				initAngle += ofRandom(45, 90);
			}
			soundEntityShoot.play();
			break;
		}
	}
}

void Entity::updateProjectiles() {//updating projectile movement and lifespans

	if (!projectiles.empty()) {

		vector<ProjectileParticle> tempProjectiles;
		for (int i = 0; i < projectiles.size(); i++) {
			if (projectiles[i].isAlive != false) {
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

void Entity::drawProjectiles() {//drawing each projectile

	if (!projectiles.empty()) {
		for (int i = 0; i < projectiles.size(); i++) {
			projectiles[i].draw();
		}
	}

}