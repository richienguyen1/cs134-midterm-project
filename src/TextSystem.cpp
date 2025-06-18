#include "TextSystem.h"

//Richie Nguyen, CS134-01 Midterm Project

/// <difficulty chart>
/// "light": max natural entities = 3, spawn every 1.5 seconds, 5% chance of champion, 0% chance of everything
/// "medium": max natural entities = 5, spawn every 1.25 seconds, 7.5% chance of champion, 0.5% chance of everything
/// "heavy": max natural entities = 5, spawn every 1 seconds, 10% chance of champion, 1% chance of everything
/// "dense": max natural entities = 7, spawn every 1 seconds, 15% chance of champion, 2.5% chance of everything
/// "ultra dense": max natural entities = 10, spawn every 0.75 seconds, 33% chance of champion, 5% chance of everything
/// "infinitely dense": max natural entities = 15, spawn every 0.5 seconds, 85% chance of champion, 15% chance of everything; not meant to be survivable
/// </difficulty chart>

TextSystem::TextSystem() {
	gameTitle = "shooting asteroids";
	gameInstructions = "use W or up arrow to move forward\nuse S or down arrow to move backwards\nuse A or left arrow to rotate left\nuse D or right arrow to rotate right\nuse the space bar to shoot at enemies\na large window is recommended\n\npress the space bar to begin";
	scoreText = "score: 0";
	titleFont.load("fonts/upheavtt.ttf", 64, false);//binding of isaac rebirth font i found online for pixel fonts
	textFont.load("fonts/VCR_OSD_MONO_1.001.ttf", 24, false);//ultrakill font i also found online for pixel fonts

}

void TextSystem::drawStartText() {
	ofSetColor(ofColor::white);
	titleFont.drawString(gameTitle, (ofGetWindowWidth() / 2.0) - titleFont.stringWidth(gameTitle) / 2.0, ofGetWindowHeight() / 2.0);
	textFont.drawString(gameInstructions, (ofGetWindowWidth() / 2.0) - titleFont.stringWidth(gameTitle) / 2.0, ofGetWindowHeight() / 2.0 + titleFont.getLineHeight());
}

void TextSystem::drawGameText(float score, int difficulty, float difficultyProgress, int lives) {
	ofSetColor(ofColor::white);
	scoreText = "score: " + to_string(score);
	switch (difficulty) {//different difficulty text depending on difficulty
	case(0)://light
		difficultyText = "enemy density: light";
		break;
	case(1)://medium
		difficultyText = "enemy density: medium";
		break;
	case(2)://heavy
		difficultyText = "enemy density: heavy";
		break;
	case(3)://dense
		difficultyText = "enemy density: dense";
		break;
	case(4)://ultra dense
		difficultyText = "enemy density: ultra dense";
		break;
	default://INFINITELY DENSE
		difficultyText = "enemy density: INFINITELY DENSE";
		break;
	}
	lifeText = "lives: " + to_string(lives);
	difficultyProgressText = "progress until next difficulty: " + to_string(difficultyProgress) + "%";
	
	textFont.drawString(scoreText, ofGetWindowWidth() / 16.0, ofGetWindowHeight() / 16.0);
	textFont.drawString(difficultyText, ofGetWindowWidth() / 16.0, ofGetWindowHeight() / 16.0 + textFont.getLineHeight());
	textFont.drawString(difficultyProgressText, ofGetWindowWidth() / 16.0, ofGetWindowHeight() / 16.0 + (2 * textFont.getLineHeight()));
	textFont.drawString(lifeText, ofGetWindowWidth() / 16.0, ofGetWindowHeight() / 16.0 + (3 * textFont.getLineHeight()));

}