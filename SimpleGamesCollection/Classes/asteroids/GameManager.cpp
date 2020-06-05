#include "asteroids/GameManager.h"
#include "asteroids/AsteroidNode.h"
#include "asteroids/ZOrderValues.h"
using namespace asteroids;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameManager::GameManager() {
  gameNode = nullptr;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameManager::~GameManager() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameManager::initAsteroids() {
  asteroidNode = AsteroidNode::create(c6);
  // sprite->setAnchorPoint(Vec2(0.5,0.5));
  asteroidNode->setPosition(500,250);
  gameNode->addChild(asteroidNode,  ZO_asteroids);

  asteroidNode->setAngle(20);//

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Node* GameManager::prepareGameNode() {
  if (gameNode != nullptr) {
    return gameNode; // never expected
  }

  if (!initGameNode()) {
    return nullptr;
  }

  if (!initAsteroids()) {
    return nullptr;
  }

  // finally
  return gameNode;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameManager::initGameNode() {

  const char filename[] = "asteroids/space_background.jpg";
  Sprite* sprite = Sprite::create(filename);
  if (sprite == nullptr) {
    C6_C2(c6, "Error while loading: ", filename);
    return false;
  }

  gameNode = sprite;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameManager::startGame() {
  asteroidNode->start();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .