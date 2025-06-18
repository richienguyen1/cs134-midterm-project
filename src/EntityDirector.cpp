
#include "EntityDirector.h"
#include "Entity.h"

//Richie Nguyen, CS134-01 Midterm Project

/// <list of movement types>
/// 0: normal; move towards the heading vector
/// 1: random; randomly move around regardless of heading vector, may use noise values
/// 2: sine/cosine; move based on a sine/cosine wave
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

/// <difficulty chart>
/// "light": max natural entities = 3, spawn every 1.5 seconds, 5% chance of champion, 0% chance of everything
/// "medium": max natural entities = 5, spawn every 1.25 seconds, 7.5% chance of champion, 0.5% chance of everything
/// "heavy": max natural entities = 5, spawn every 1 seconds, 10% chance of champion, 1% chance of everything
/// "dense": max natural entities = 7, spawn every 1 seconds, 15% chance of champion, 2.5% chance of everything
/// "ultra dense": max natural entities = 10, spawn every 0.75 seconds, 33% chance of champion, 5% chance of everything
/// "infinitely dense": max natural entities = 15, spawn every 0.5 seconds, 85% chance of champion, 15% chance of everything; not meant to be survivable
/// </difficulty chart>

EntityDirector::EntityDirector() {
	entityList.clear();
	timer = 0.0;
	timeUntilSpawn = 1.5;
	difficulty = 0;
	difficultyProgress = 0.0;
	maxNatEntities = 3;
	soundEntityBump.load("sounds/entityBump.mp3");
	soundEntityBump.setVolume(0.1);
	soundEntityBump.setMultiPlay(true);
	soundEntityBreak.load("sounds/entityBreak.mp3");
	soundEntityBreak.setVolume(0.30);
	soundEntityBreak.setMultiPlay(true);
	soundEntitySpawn.load("sounds/entitySpawn.mp3");
	soundEntitySpawn.setVolume(0.25);
	soundEntitySpawn.setMultiPlay(true);
	soundEntityChampion.load("sounds/entityChamp.mp3");
	soundEntityChampion.setVolume(0.25);
	soundEntityChampion.setMultiPlay(true);
	soundEntityChampionEverything.load("sounds/entityChamp.mp3");
	soundEntityChampionEverything.setVolume(0.33);
	soundEntityChampionEverything.setSpeed(0.66);
	soundEntityChampionEverything.setMultiPlay(true);
	soundNewDifficulty.load("sounds/entityBump.mp3");
	soundNewDifficulty.setVolume(0.5);
	soundNewDifficulty.setSpeed(0.7);
	soundNewDifficulty.setMultiPlay(true);
}

Entity EntityDirector::generateEntityProperties(float initRadius) {//create a new entity with given properties

	//first initialization of entity properties
	glm::vec3 entityPosition = glm::vec3(ofRandom(ofGetWindowWidth() / 16.0, 15 * ofGetWindowWidth() / 16.0), ofRandom(ofGetWindowHeight() / 16.0, 15 * ofGetWindowHeight() / 16.0), 0.0);
	float entityMass = ofRandom(1.0, 4.0);
	int entityMovement = glm::round(ofRandom(0, 1));
	int entityChampion = championChance(difficulty);
	ofColor entityInitColor = ofColor(255, 255, 255, 0);//temporary color before color changes based on champion
	switch (entityChampion) {//determining color based on champion
	case(0)://normal
		entityInitColor = ofColor(ofRandom(180, 255), ofRandom(180, 255), ofRandom(180, 255), 0.0);
		break;
	case(1)://large split
		entityInitColor = ofColor(ofColor::goldenRod.r, ofColor::goldenRod.g, ofColor::goldenRod.b, 0.0);
		break;
	case(2)://+ shot
		entityInitColor = ofColor(ofColor::orangeRed.r, ofColor::orangeRed.g, ofColor::orangeRed.b, 0.0);
		break;
	case(3)://x shot
		entityInitColor = ofColor(ofColor::orange.r, ofColor::orange.g, ofColor::orange.b, 0.0);
		break;
	case(4)://radial spread
		entityInitColor = ofColor(ofColor::hotPink.r, ofColor::hotPink.g, ofColor::hotPink.b, 0.0);
		break;
	case(5)://ghost
		entityInitColor = ofColor(ofColor::gray.r, ofColor::gray.g, ofColor::gray.b, 0.0);
		break;
	default://everything
		entityInitColor = ofColor(ofRandom(225, 255), ofRandom(225, 255), ofRandom(225, 255), 0.0);
		break;	
	}

	//initializing entity shape
	vector<glm::vec3> entityVerts;
	float minRadius = ofRandom(initRadius / 3.0, initRadius / 1.5);//minimum radius
	int entitySides = glm::round(ofRandom(10, 26));//randomly select amount of verticies, 10-25
	float tempAngle = 0.0;
	float tempAngleIncrement = ofDegToRad(360.0) / entitySides;
	float entityRadius = 0;
	for (int i = 0; i < entitySides; i++) {
		float randRadius = ofRandom(minRadius, initRadius);
		glm::vec3 generatedVertex = glm::vec3(glm::cos(tempAngle) * randRadius, glm::sin(tempAngle) * randRadius, 0.0);//creating vertex based on polar coordinates and radius variation
		entityVerts.push_back(generatedVertex);
		tempAngle += tempAngleIncrement;
		entityRadius += randRadius;
	}
	entityRadius = entityRadius / entitySides;

	Entity newEntity = Entity(entityPosition, entityRadius, entityInitColor, entityMass, entityMovement, entityChampion, entityVerts);
	newEntity.collidedCount = 0;
	cout << "generated entity with champion type " << newEntity.championType << endl;
	return newEntity;
}

void EntityDirector::spawnEntity() {//spawning entities into the game
	
	float initRadius = ofRandom(25.0, 100.0);
	Entity newEntity = generateEntityProperties(initRadius);//generate an entity
	if (newEntity.championType > 5) {//everything champion
		soundEntityChampionEverything.play();
	}
	else if ((newEntity.championType > 0) && (newEntity.championType < 6)) {//normal champions
		soundEntityChampion.play();
	}
	else {//no champions
		soundEntitySpawn.play();
	}
	entityList.push_back(newEntity);

}

void EntityDirector::removeEntity() {//removing an entity and generating smaller entities if possible
	vector<Entity> tempList;
	for (int i = 0; i < entityList.size(); i++) {
		if (entityList[i].isAlive == true) {//if entity is alive
			tempList.push_back(entityList[i]);//transfer entity to new list
		}
		else {//entity is not alive
			soundEntityBreak.play();
			int spawnedChildren = glm::round(ofRandom(3, 5));//default amount of children spawned
			float childRadius = entityList[i].radius / spawnedChildren;
			if ((entityList[i].championType == 1) || (entityList[i].championType == 6)) {//if champion allows large splits
				spawnedChildren = 8;
				childRadius = entityList[i].radius / 6.0;
			}
			if (childRadius >= childRadiusMin) {//if the resulting child radius is larger than the minimum radius threshold
				float childAngle = ofRandom(-45.0, 45.0);
				childRadius = entityList[i].radius * 0.66;
				for (int j = 0; j < spawnedChildren; j++) {
					Entity child = generateEntityProperties(childRadius);
					child.angle = childAngle;
					child.position = entityList[i].position;
					child.mass = entityList[i].mass * 0.8;
					if (entityList[i].championType == 6) {//if parent entity champion type is everything
						child.championType = 6;//inherent everything champion type
					}
					else {
						child.championType = 0;//set to none otherwise
					}
					child.hasParent = true;
					child.forces += child.heading() * 750.0;;
					child.velocity = child.heading() * 250.0;
					tempList.push_back(child);
					childAngle += (ofRandom(300, 350) / spawnedChildren);
				}
			}
			float difficultyMultiplier = 1.0;
			switch (difficulty) {//multipliers based on current difficulty
			case (0)://light
				difficultyMultiplier = 1.0;
				break;
			case(1)://medium
				difficultyMultiplier = 1.25;
				break;
			case(2)://heavy
				difficultyMultiplier = 1.5;
				break;
			case(3)://dense
				difficultyMultiplier = 2.5;
				break;
			case(4)://ultra dense
				difficultyMultiplier = 5.0;
				break;
			default://INFINITELY DENSE
				difficultyMultiplier = 10.0;
				break;
			}
			float championMultiplier = 1.0;
			switch (entityList[i].championType) {
			case (0)://no champions
				championMultiplier = 1.0;
				difficultyProgress += (2.0 + (0.5 * ofNoise(difficultyProgress)));//increase difficulty progress up to 2.5
				break;
			case (1, 2, 3, 4, 5)://all champions but everything
				championMultiplier = ofRandom(1.5, 3.0);
				difficultyProgress += (5.0 + (1.5 * ofNoise(difficultyProgress)));//increase difficulty progress up to 6.5
				break;
			default://everything
				championMultiplier = 7.5;
				difficultyProgress += (10.0 + (5.0 * ofNoise(difficultyProgress)));//increase difficulty progress up to 15
				break;
			}
			score += glm::ceil(5 * entityList[i].verts.size() * difficultyMultiplier * championMultiplier);//score based on difficulty, vertices, and champion type
			if ((entityList[i].hasParent == false) && (natEntities > 0)) {
				natEntities--;
			}
		}
	}
	entityList = tempList;
}

void EntityDirector::updateEntities() {//movement and collision for all entities
	for (int i = 0; i < entityList.size(); i++) {
		entityList[i].lifespan += 1 / ofGetFrameRate();//update lifespan
		entityList[i].timeSinceCollision += 1 / ofGetFrameRate();//update time since collision
		entityList[i].shootTimer += 1 / ofGetFrameRate();//update timer for shooting
		entityList[i].wrapPosition();
		entityList[i].beOpaque();//making entity opaque

		if (entityList.size() > 1) {//entity to entity collision requires more than 1 entity
			for (int j = i + 1; j < entityList.size(); j++) {//entity to entity collision
				if ((entityList[i].championType < 5) && (entityList[j].championType < 5)) {//only applies to entities without ghost champion type
					bool entitiesCollided = entityList[i].intersect(entityList[j].position, entityList[j].radius, entityList[j].isTangible);
					if (entitiesCollided == true) {
						soundEntityBump.play();

						entityList[i].timeSinceCollision = 0.0;
						entityList[j].timeSinceCollision = 0.0;
						entityList[i].isTangible = false;
						entityList[j].isTangible = false;
						entityList[i].color.a -= 50.0;
						entityList[j].color.a -= 50.0;
						entityList[i].collidedCount++;
						entityList[j].collidedCount++;//updating amount of collisions within time frame

						if ((entityList[i].collidedCount <= 2) || (entityList[j].collidedCount <= 2)) {//if less than 3 collisions within threshold time frame
							entityList[i].position -= entityList[i].heading() * ((entityList[j].radius * 1.5) + ofNoise(entityList[i].lifespan));
							entityList[j].position -= entityList[j].heading() * ((entityList[i].radius * 1.5) + ofNoise(entityList[j].lifespan));//move entities based on radius

						}
						else {//more than 2 collisions within threshold time frame
							entityList[i].color.a = 100.0;
							entityList[j].color.a = 100.0;
							entityList[i].position = glm::vec3(ofRandom(0.0, ofGetWindowWidth()), ofRandom(0.0, ofGetWindowHeight()), 0.0);//set random position instead
							entityList[j].position = glm::vec3(ofRandom(0.0, ofGetWindowWidth()), ofRandom(0.0, ofGetWindowHeight()), 0.0);
						}

						entityList[i].angle += 180.0;
						entityList[j].angle += 180.0;
						entityList[i].forces += entityList[i].heading() * entityList[i].velocity.length() * entityList[i].mass;
						entityList[j].forces += entityList[j].heading() * entityList[j].velocity.length() * entityList[j].mass;
						entityList[i].velocity = entityList[i].heading() * 0.25;
						entityList[j].velocity = entityList[j].heading() * 0.25;//movement after collision

					}
				}
			}
		}

		//projectile collision

		if ((entityList[i].color.a >= 250.0) && (entityList[i].isTangible != true)) {//making entity tangible if alpha is more than 250
			entityList[i].isTangible = true;
		}
		if ((entityList[i].collidedCount != 0) && (entityList[i].timeSinceCollision >= collisionThreshold)) {//resetting collision count if not collisions within threshold
			//cout << "reset collision" << endl;
			entityList[i].collidedCount = 0;
		}
		
		//entityList[i].rotateLeft();
		entityList[i].move();//moving entity
		if ((entityList[i].velocity != glm::vec3(0, 0, 0)) || (entityList[i].rotVel != 0.0)) {
			entityList[i].integrate();
		}

		entityList[i].updateProjectiles();
		entityList[i].shootProjectiles();
		
	}
}

void EntityDirector::drawEntities() {//drawing entities
	for (int i = 0; i < entityList.size(); i++) {
		entityList[i].draw();
		entityList[i].drawProjectiles();
	}
}

void EntityDirector::spawningSystem() {//automated spawning system for entities

	if (timer < timeUntilSpawn) {
		timer += (1.0 / ofGetFrameRate());
	}
	else {
		if (natEntities < maxNatEntities) {
			spawnEntity();
			natEntities++;
		}
		
		timer = 0.0;
	}

}

void EntityDirector::resetEntities() {//removing all entities and setting back difficulty for compensation
	entityList.clear();
	natEntities = 0;
	if (difficulty > 0) {
		difficulty--;
	}
	difficultyProgress *= 0.33;
}

int EntityDirector::championChance(int inputDifficulty) {//calculate the chances and returns a champion type based on difficulty
	int outputChampion = 0;
	float tempRand = ofRandom(0, 100.0);
	switch (inputDifficulty) {//selecting champion based on difficulty
	case (0)://5% chance of champion, 0% chance of everything
		if (tempRand >= 95.0) {//5% champion
			outputChampion = glm::round(ofRandom(1, 5));
		}
		else {
			outputChampion = 0;
		}
		break;
	case (1)://7.5% chance of champion, 0.5% chance of everything
		if ((tempRand >= 92.0) && (tempRand < 99.5)) {//7.5% champion
			outputChampion = glm::round(ofRandom(1, 5));
		}
		else if (tempRand >= 99.5) {//0.5% everything
			outputChampion = 6;
		}
		else {
			outputChampion = 0;
		}
		break;
	case (2)://10% chance of champion, 1% chance of everything
		if ((tempRand >= 89.0) && (tempRand < 99.0)) {//10% champion
			outputChampion = glm::round(ofRandom(1, 5));
		}
		else if (tempRand >= 99.0) {//0.5% everything
			outputChampion = 6;
		}
		else {
			outputChampion = 0;
		}
		break;
	case (3)://15% chance of champion, 2.5% chance of everything
		if ((tempRand >= 82.5) && (tempRand < 97.5)) {//15% champion
			outputChampion = glm::round(ofRandom(1, 5));
		}
		else if (tempRand >= 97.5) {//2.5% everything
			outputChampion = 6;
		}
		else {
			outputChampion = 0;
		}
		break;
	case (4)://33% chance of champion, 5% chance of everything
		if ((tempRand >= 62) && (tempRand < 95.0)) {//33% champion
			outputChampion = glm::round(ofRandom(1, 5));
		}
		else if (tempRand >= 95.0) {//5% everything
			outputChampion = 6;
		}
		else {
			outputChampion = 0;
		}
		break;
	default://85% chance of champion, 15% chance of everything; not meant to be survivable
		if (tempRand < 85.0) {//85% champion
			outputChampion = glm::round(ofRandom(1, 5));
		}
		else {//15% everything
			outputChampion = 6;
		}
		break;
	}
	return outputChampion;
		
}

void EntityDirector::updateDifficulty() {
	if (difficultyProgress >= 100) {
		difficulty++;
		soundNewDifficulty.play();
		if (difficulty <= 4) {
			difficultyProgress = 0.0;
		}
	}
	else {
		if (difficulty <= 4) {
			difficultyProgress += 0.0185;
		}
		
	}

	switch (difficulty) {//setting spawning stats based on difficulty
	case (0)://light
		timeUntilSpawn = 1.5;
		maxNatEntities = 3;
		break;
	case(1)://medium
		timeUntilSpawn = 1.25;
		maxNatEntities = 5;
		break;
	case(2)://heavy
		timeUntilSpawn = 1.0;
		maxNatEntities = 5;
		break;
	case(3)://dense
		timeUntilSpawn = 1.0;
		maxNatEntities = 7;
		break;
	case(4)://ultra dense
		timeUntilSpawn = 0.75;
		maxNatEntities = 10;
		break;
	default://INFINITELY DENSE
		timeUntilSpawn = 0.5;
		maxNatEntities = 15;
		break;
	}

}
