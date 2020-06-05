#include "asteroids/GameManager.h"
#include "asteroids/AsteroidNode.h"
#include "asteroids/SpaceshipNode.h"
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

  asteroidNode->setPosition(500,250);
  gameNode->addChild(asteroidNode,  ZO_asteroids);

  asteroidNode->setAngle(20);//

  return true;
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

bool GameManager::initSpaceship() {

  spaceshipNode = SpaceshipNode::create(c6);
  if (spaceshipNode == nullptr) {
    return false;
  }

  spaceshipNode->setPosition(200,550);
  gameNode->addChild(spaceshipNode,  ZO_ss);

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

  if (!initSpaceship()) {
    return nullptr;
  }

  // finally
  return gameNode;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameManager::startGame() {
  asteroidNode->start();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameManager::processKeyCode(const EventKeyboard::KeyCode keyCode) {

  if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
    spaceshipNode->rotateClockwice();
    return true;
  }
  else if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
    spaceshipNode->rotateCounterClockwice();
    return true;
  }

  return false;
}




// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .