#pragma once

#include "ofMain.h"
#include "Entity.h"

//Richie Nguyen, CS134-01 Midterm Project

class EntityDirector
{
public:

	EntityDirector();

	Entity generateEntityProperties(float initRadius);
	void spawnEntity();
	void removeEntity();
	void updateEntities();
	void drawEntities();
	void spawningSystem();
	void resetEntities();
	int championChance(int inputDifficulty);
	void updateDifficulty();


	vector<Entity> entityList;
	float collisionThreshold = 10 / ofGetFrameRate();//predefined to be 10 frames; if 3 collisions happen within 10 frames between entities then positions reset
	int difficulty = 0;
	float difficultyProgress = 0.0;//should take around 90 seconds to go to next difficulty tier without doing anything, around 0.0185 added per frame out of 100
	float timeUntilSpawn = 1.5;//initial timer to spawn entities
	float timer = 0;
	int maxNatEntities = 3;//initial maximimum amount of naturally spawned (example: not spawned from parent) entities at once
	int natEntities = 0;
	float childRadiusMin = 7.5;//lowest possible child radius an entity can create before being fully deleted
	float score = 0;
	ofSoundPlayer soundEntityBump;//sounds modified from freesound.org/people/14GPanskaValaChristoffer/sounds/420171/ and freesound.org/people/KorgMS2000B/sounds/54411/
	ofSoundPlayer soundEntityBreak;//sounds modified from freesound.org/people/160033/sounds/366179/ and freesound.org/people/iliyabylich04/sounds/651325/
	ofSoundPlayer soundEntitySpawn;//sounds modified from freesound.org/people/oceansonmars/sounds/716623/ and freesound.org/people/stomachache/sounds/44861/
	ofSoundPlayer soundEntityChampion;//sound modified from freesound.org/people/kevp888/sounds/710378/
	ofSoundPlayer soundEntityChampionEverything;//sound modified from freesound.org/people/kevp888/sounds/710378/
	ofSoundPlayer soundNewDifficulty;//sounds modified from freesound.org/people/14GPanskaValaChristoffer/sounds/420171/ and freesound.org/people/KorgMS2000B/sounds/54411/
	


};

