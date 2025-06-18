#include "ofApp.h"

//Richie Nguyen, CS134-01 Midterm Project

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofSetBackgroundColor(ofColor::black);

	player = new Player();
	text = new TextSystem();
	director = new EntityDirector();
	explosion = new Explosion();
	player->isAlive = true;
	player->position = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0.0);
	explosion->position = player->position;
	director->timer = 0.0;
	respawnTimer = 0.0;
	lives = 3;
	director->score = 0.0;

}

//--------------------------------------------------------------
void ofApp::update(){

	if (isGameActive == true) {//game started

		if ((player->isAlive != true) && (explosion->hasFired != true)) {//if player is not alive and explosion has not happened
			explosion->position = player->position;
			explosion->explodeParticles();
			respawnTimer = 0.0;
		}

		if (player->isAlive != true) {
			if (respawnTimer >= timeUntilRespawn) {//respawning player and resetting entities
				respawnTimer = 0.0;
				lives--;
				if (lives >= 1) {//respawn player
					player->position = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0.0);
					player->angle = 0.0;
					player->rotVel = 0.0;
					player->velocity = player->heading() * 0.0001;
					player->forces = glm::vec3(0.0, 0.0, 0.0);
					player->acceleration = glm::vec3(0.0, 0.0, 0.0);
					director->resetEntities();
					director->timer = 0.0;
					director->natEntities = 0;
					explosion->resetParticles();

					player->isAlive = true;
					explosion->hasFired = false;
					explosion->firedOnce = false;
				}
				else if (lives == 0) {//reset game; game over
					isGameActive = false;
					director->resetEntities();
					director->timer = 0.0;
					director->natEntities = 0;
					director->difficulty = 0;
					explosion->resetParticles();
					explosion->hasFired = false;
					explosion->firedOnce = false;
					respawnTimer = 0.0;
					lives = 3;
					player->position = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0.0);
					player->angle = 0.0;
					player->rotVel = 0.0;
					player->velocity = player->heading() * 0.0001;
					player->forces = glm::vec3(0.0, 0.0, 0.0);
					player->acceleration = glm::vec3(0.0, 0.0, 0.0);
					player->isAlive = true;
				}
				
			}
			else {
				respawnTimer += 1 / ofGetFrameRate();
			}
		}

		if (player->isAlive == true) {//runs as long the player is alive

			player->wrapPosition();//making the player wrap around the window boundaries
			player->beOpaque();

			//movement for the player
			if (keymap[OF_KEY_LEFT] || keymap[97]) {//left arrow or a
				player->rotateLeft();
			}
			else if (keymap[OF_KEY_RIGHT] || keymap[100]) {//right arrow or d
				player->rotateRight();
			}
			else if (keymap[OF_KEY_UP] || keymap[119]) {//up arrow or w
				player->moveForward();
			}
			else if (keymap[OF_KEY_DOWN] || keymap[115]) {//down arrow or s
				player->moveBackward();
			}

			//player moves around with physics integration
			if ((player->velocity != ofVec3f(0.0, 0.0, 0.0)) || (player->rotVel != 0.0)) {
				player->integrate();
			}

			if (!director->entityList.empty()) {//collision checking involving entities
				for (int i = 0; i < director->entityList.size(); i++) {

					player->checkCollision(director->entityList[i].position, director->entityList[i].radius, director->entityList[i].isTangible);//checking player collision with all entities
					if (!director->entityList[i].projectiles.empty()) {
						for (int j = 0; j < director->entityList[i].projectiles.size(); j++) {
							player->checkCollision(director->entityList[i].projectiles[j]);//checking player collision with enemy projectiles
						}
					}
					
					if (!player->projectiles.empty()) {//checking player projectile collision with entities
						for (int j = 0; j < player->projectiles.size(); j++) {
							bool projectileCheck = director->entityList[i].checkCollision(player->projectiles[j]);//checking player collision with enemy projectiles
							if (projectileCheck == true) {//if collision did happen, removing projectile
								player->projectiles[j].isAlive = false;
							}
						}
					}
				}
			}
		}

		director->spawningSystem();
		director->removeEntity();
		director->updateEntities();//updating movement and window wrap for entities
		director->updateDifficulty();
		player->updateProjectiles();
		explosion->updateParticles();//updating particle movement for player explosion
		
	}
	else {//game not started
		director->timer = 0.0;
		lives = 3;
		director->resetEntities();
		director->natEntities = 0;
		director->difficulty = 0;
		explosion->resetParticles();
		explosion->hasFired = false;
		explosion->firedOnce = false;
		player->isAlive = true;
		player->position = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0.0);
		player->angle = 0.0;
		player->rotVel = 0.0;
		player->velocity = player->heading() * 0.0001;
		player->forces = glm::vec3(0.0, 0.0, 0.0);
		player->acceleration = glm::vec3(0.0, 0.0, 0.0);
	}
	
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(ofColor::white);
	background.draw(0,0);
	if (isGameActive == true) {//draw game elements while active
		player->drawProjectiles();
		if (player->isAlive != false) {
			player->draw();
		}
		explosion->drawParticles();
		director->drawEntities();
		text->drawGameText(director->score, director->difficulty, director->difficultyProgress, lives);
	}
	else {//draw menu text otherwise
		text->drawStartText();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	keymap[key] = true;
	if (key == ' ') {
		if (isGameActive == !true) {//pressing space at the main menu starts the game
			director->score = 0.0;
			isGameActive = true;
		}
		else {
			if (player->isAlive != false) {
				cout << "fired projectile" << endl;
				player->shootProjectiles();
			}
			
		}
	}

	//if (key == 'f') {
		//director->spawnEntity();//debug to manually spawn entities
	//}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	keymap[key] = false;
	if ((key == OF_KEY_UP) || (key == OF_KEY_DOWN) || (key == 119) || (key == 115)) {//releasing the forward/backwards movement stops the thrust sound
		player->soundThrust.stop();
	}
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
