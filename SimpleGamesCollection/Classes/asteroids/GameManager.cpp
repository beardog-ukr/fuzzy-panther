#include "asteroids/GameManager.h"
#include "asteroids/AsteroidNode.h"
#include "asteroids/GameEndScene.h"
#include "asteroids/LaserNode.h"
#include "asteroids/SpaceshipNode.h"
#include "asteroids/ZOrderValues.h"
using namespace asteroids;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

static const float splashDelay = 3;


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

  FadeIn* fin = FadeIn::create(splashDelay/2);
  FadeOut* fout = FadeOut::create(splashDelay/2);
  Sequence* seq = Sequence::create(fin, fout, RemoveSelf::create(), nullptr);

  splashSprite->runAction(seq);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameManager::initAsteroids() {
  asteroidNode = AsteroidNode::create(20, AsteroidNode::RT_medium, c6);

  asteroidNode->setPosition(50,250);
  gameNode->addChild(asteroidNode,  ZO_asteroid);
  asteroidNode->start();

  asteroidNode = AsteroidNode::create(20, AsteroidNode::RT_medium, c6);
  asteroidNode->setPosition(500,250);
  gameNode->addChild(asteroidNode,  ZO_asteroid);
  asteroidNode->start();

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
    C6_C1(c6, "Failed to init spaceship");
    return false;
  }

  spaceshipNode->setPosition(320.0,450.0);
  gameNode->addChild(spaceshipNode,  ZO_ss);
  C6_C2(c6, "Added spaceship to ", "320x450" );//spaceshipNode->getPosition().x);

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
  //asteroidNode->start();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameManager::processContact(PhysicsContact& contact) {
  C6_D1(c6, "here");

  Node* nodeA = contact.getShapeA()->getBody()->getNode();
  Node* nodeB = contact.getShapeB()->getBody()->getNode();
  if (nodeB ==nullptr) {
    C6_I1(c6, "Bad node B:");
    return true;
  }

  Node* laserAgent = nullptr;
  Node* asteroidAgent = nullptr;
  Node* shipAgent = nullptr;
  switch( nodeA->getTag()) {
  case IT_laser:
    laserAgent = nodeA;
    C6_D1(c6, "Node A is laser");
    break;
  case IT_asteroid:
    asteroidAgent = nodeA;
    C6_D1(c6, "Node A is asteroid");
    break;
  case IT_ship:
    shipAgent = nodeA;
    C6_D1(c6, "Node A is ss");
    break;
  default:
    C6_I2(c6, "Bad call A:", (int)nodeA->getTag());
    return true;
  }

  switch( (int)nodeB->getTag()) {
  case IT_laser:
    if (laserAgent != nullptr) {
      C6_D1(c6, "Both are lasers");
    }
    laserAgent = nodeB;

    break;
  case IT_asteroid:
    asteroidAgent = nodeB;
    C6_D1(c6, "Node B is asteroid");
    break;
  case IT_ship:
    shipAgent = nodeB;
    break;
  default:
    C6_I2(c6, "Bad call B:", (int)nodeB->getTag());
    return true;
  }

  if (laserAgent == nullptr) {
    //ship vs asteroid, game over
    C6_I1(c6, "Game Over");

    Node* shipParent = shipAgent->getParent();
    Node* asteroidParent = asteroidAgent->getParent();
    addSplashAt(shipParent->getPosition(), asteroidParent->getPosition());

    shipParent->stopAllActions();
    shipAgent->runAction(FadeOut::create(splashDelay));

    asteroidParent->stopAllActions();
    asteroidAgent->runAction(FadeOut::create(splashDelay));

    CallFunc *cf = CallFunc::create([this]() {
      this->processGameOver();
    });

    Sequence* seq = Sequence::create(DelayTime::create(splashDelay*1.1), cf, nullptr);
    gameNode->runAction(seq);
  }
  else if (shipAgent == nullptr) {
    //laser vs asteroid
    //remove both
    Node* laserParent = laserAgent->getParent();
    Node* asteroidParent = asteroidAgent->getParent();

    addSplashAt(laserParent->getPosition(), asteroidParent->getPosition());
    laserAgent->removeFromParentAndCleanup(true);
    asteroidAgent->removeFromParentAndCleanup(true);
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameManager::processGameOver() {
  C6_D1(c6, "here");

  gameNode->removeAllChildren();

  // asteroidNode->removeFromParentAndCleanup(true);
  // asteroidNode = nullptr;
  // spaceshipNode->removeFromParentAndCleanup(true);
  // spaceshipNode = nullptr;

  initAsteroids();
  initSpaceship();

  startGame();

  Scene* gameEndScene = GameEndScene::createScene(false, c6);
  Director::getInstance()->pushScene(gameEndScene);
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