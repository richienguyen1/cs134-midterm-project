#pragma once

#include "ofMain.h"
#include "Shape.h"
#include "Player.h"
#include "TextSystem.h"
#include "EntityDirector.h"
#include "Explosion.h"

//Richie Nguyen, CS134-01 Midterm Project

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		map<int, bool> keymap;
		bool isGameActive = false;
		float respawnTimer = 0.0;
		float timeUntilRespawn = 3.0;

		ofImage background = ofImage("images/background.png");

		Player *player = NULL;
		TextSystem *text = NULL;
		EntityDirector *director = NULL;
		Explosion* explosion = NULL;
		
		int lives = 3;


};
