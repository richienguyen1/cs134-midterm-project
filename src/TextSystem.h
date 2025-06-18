#pragma once
#include "ofMain.h"

//Richie Nguyen, CS134-01 Midterm Project

class TextSystem
{
public:

	TextSystem();
	void drawStartText();
	void drawGameText(float score, int difficulty, float difficultyProgress, int lives);

	ofTrueTypeFont titleFont;
	string gameTitle;
	ofTrueTypeFont textFont;
	string gameInstructions;
	string scoreText;
	string difficultyText;
	string difficultyProgressText;
	string lifeText;

};

