#include "asteroids/GameManager.h"
#include "asteroids/AsteroidNode.h"
#include "asteroids/LaserNode.h"
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

void GameManager::addLaser() {
  double ra = spaceshipNode->getAngle();
  C6_D2(c6, "Rotation angle is ", ra);
  LaserNode* laserNode = LaserNode::create(ra, c6);
  if (laserNode == nullptr) {
    return;
  }

  laserNode->setPosition(spaceshipNode->getPosition());
  gameNode->addChild(laserNode, ZO_laser);

  laserNode->start();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameManager::addSplashAt(const Vec2& pointA, const Vec2& pointB) {
  const string filename = "asteroids/laserBlue11.png";
  Sprite* splashSprite = Sprite::create(filename);
  if (splashSprite == nullptr) {
    C6_D2(c6, "Failed to find file ", filename);
    return;
  }

  Vec2 splashPos = {.x = (pointA.x + pointB.x)/2, .y = (pointA.y + pointB.y)/2 };
  C6_D4(c6, "Point A is ", pointA.x, ":", pointA.x);
  C6_D4(c6, "Point B is ", pointB.x, ":", pointB.x);
  C6_D4(c6, "Middle is ", splashPos.x, ":", splashPos.x);

  splashSprite->setPosition(splashPos);
  gameNode->addChild(splashSprite, ZO_laser);

  splashSprite->setOpacity(0);

  FadeIn* fin = FadeIn::create(1.5);
  FadeOut* fout = FadeOut::create(1.5);
  Sequence* seq = Sequence::create(fin, fout, RemoveSelf::create(), nullptr);

  splashSprite->runAction(seq);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameManager::initAsteroids() {
  asteroidNode = AsteroidNode::create(20, AsteroidNode::RT_big, c6);

  asteroidNode->setPosition(400,550);
  gameNode->addChild(asteroidNode,  ZO_asteroid);

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

  // sprite->setOpacity(10);

  gameNode = sprite;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameManager::initSpaceship() {

  spaceshipNode = SpaceshipNode::create(c6);
  if (spaceshipNode == nullptr) {
    return false;
  }

  spaceshipNode->setPosition(300,250);
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
  // asteroidNode->start();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameManager::processContact(PhysicsContact& contact) {
  printf("%s: here\n", __func__);

  Node* nodeA = contact.getShapeA()->getBody()->getNode();
  Node* nodeB = contact.getShapeB()->getBody()->getNode();

  Node* laserAgent = nullptr;
  Node* asteroidAgent = nullptr;
  Node* shipAgent = nullptr;
  switch( nodeA->getTag()) {
  case IT_laser:
    laserAgent = nodeA;
    break;
  case IT_asteroid:
    asteroidAgent = nodeA;
    break;
  case IT_ship:
    shipAgent = nodeA;
  default:
    C6_I2(c6, "Bad call A:", (int)nodeA->getTag());
    return true;
  }

  switch( nodeB->getTag()) {
  case IT_laser:
    laserAgent = nodeB;
    break;
  case IT_asteroid:
    asteroidAgent = nodeB;
    break;
  case IT_ship:
    shipAgent = nodeB;
  default:
    C6_I2(c6, "Bad call B:", (int)nodeA->getTag());
    return true;
  }

  if (laserAgent == nullptr) {
    //ship vs asteroid, game over
    C6_I1(c6, "Game Over");

    // nodeA->removeFromParentAndCleanup(true);
    // nodeB->removeFromParentAndCleanup(true);


  }
  else {
    Node* laserParent = laserAgent->getParent();
    Node* asteroidParent = asteroidAgent->getParent();




    addSplashAt(laserParent->getPosition(), asteroidParent->getPosition());
    laserAgent->removeFromParentAndCleanup(true);
    asteroidAgent->removeFromParentAndCleanup(true);
  }

  return true;
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
  else if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
    spaceshipNode->moveForward();
    return true;
  }
  else if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
    addLaser();
    return true;
  }

  return false;
}




// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .