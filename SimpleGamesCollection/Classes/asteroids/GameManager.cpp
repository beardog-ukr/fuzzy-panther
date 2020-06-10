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

enum GameManagerActionTags {
  GMAT_processing_game_over = 10
};

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

void GameManager::addTwoAsteroids(const double currentAngle, const cocos2d::Vec2& position,
                                  const RockType type) {
  double newAngleGenerator = currentAngle -10;

  for (int i = 0; i<2; i++) {
    AsteroidNode* asteroidNode = AsteroidNode::create(newAngleGenerator, type, c6);
    newAngleGenerator += 20;
    asteroidNode->setPosition(position.x, position.y);
    gameNode->addChild(asteroidNode,  ZO_asteroid);
    asteroidNode->start();

    asteroidNodes.insert({asteroidNode->getName(), asteroidNode});
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameManager::evaluateContactAgents(Node * nodeA, Node * nodeB,
                                        Node*& laserAgent, Node*& asteroidAgent, Node*& shipAgent) {
  laserAgent = nullptr;
  asteroidAgent = nullptr;
  shipAgent = nullptr;

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
    return false;
  }

  switch( (int)nodeB->getTag()) {
  case IT_laser:
    if (laserAgent != nullptr) {
      C6_D1(c6, "Both are lasers");
    }
    laserAgent = nodeB;

    break;
  case IT_asteroid:
    if (asteroidAgent!=nullptr) {
      C6_D1(c6, "Both are asteroids");
    }
    asteroidAgent = nodeB;
    break;
  case IT_ship:
    shipAgent = nodeB;
    break;
  default:
    C6_I2(c6, "Bad call B:", (int)nodeB->getTag());
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameManager::initAsteroids() {
  asteroidNodes.clear();

  const int initialCount = 2;
  Vec2 points[initialCount] = {
    //{.x = 400, .y = 250}, {.x = 500, .y = 150}
    {.x = 300, .y = 250}, {.x = 300, .y = 240}
  };

  const RockType types[initialCount] = {RT_medium, RT_medium};

  for (int i = 0; i<initialCount; i++) {
    AsteroidNode* asteroidNode = AsteroidNode::create(20, types[i], c6);
    asteroidNode->setPosition(points[i].x, points[i].y);
    gameNode->addChild(asteroidNode,  ZO_asteroid);
    asteroidNode->start();

    asteroidNodes.insert({asteroidNode->getName(), asteroidNode});
  }

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
  if (!evaluateContactAgents(nodeA, nodeB,laserAgent, asteroidAgent, shipAgent)) {
    return true;
  }

  if ((shipAgent != nullptr)&&(asteroidAgent!= nullptr)) {
    //ship vs asteroid, game over
    C6_D1(c6, "Game Over");

    processGameOverStage1(asteroidAgent);
  }
  else if ((laserAgent != nullptr)&&(asteroidAgent!= nullptr)) {
    //laser vs asteroid

    Node* laserParent = laserAgent->getParent();
    Node* asteroidParent = asteroidAgent->getParent();

    CallFunc *cf = CallFunc::create([this, laserParent, asteroidParent]() {
      processLaserVsAsteroid(laserParent, asteroidParent);
    });
    Sequence* seq = Sequence::create(DelayTime::create(0.1), cf, nullptr);
    gameNode->runAction(seq);
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameManager::processGameOver(const bool victory) {
  C6_D1(c6, "here");

  gameNode->removeAllChildren();

  initAsteroids();
  initSpaceship();

  startGame();

  Scene* gameEndScene = GameEndScene::createScene(victory, c6);
  Director::getInstance()->pushScene(gameEndScene);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameManager::processGameOverStage1(cocos2d::Node* asteroidAgent) {
  Action const * pgoAction = gameNode->getActionByTag(GMAT_processing_game_over);
  if (pgoAction != nullptr) {
    C6_D1(c6, "Game over request skipped, there is one already");
    return;
  }

  // Node* shipParent = shipAgent->getParent();
  Node* asteroidParent = asteroidAgent->getParent();
  addSplashAt(spaceshipNode->getPosition(), asteroidParent->getPosition());

  // spaceshipNode->doDie();

  asteroidParent->stopAllActions();
  asteroidAgent->runAction(FadeOut::create(splashDelay));

  CallFunc *cf = CallFunc::create([this]() {
    this->processGameOver(false);
  });

  Sequence* seq = Sequence::create(DelayTime::create(splashDelay*1.1), cf, nullptr);
  seq->setTag(GMAT_processing_game_over);
  gameNode->runAction(seq);

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

void GameManager::processLaserVsAsteroid(Node*const laserNode, Node*const asteroidNode ) {
  addSplashAt(laserNode->getPosition(), asteroidNode->getPosition());
  laserNode->removeFromParentAndCleanup(true);

  auto afr = asteroidNodes.find(asteroidNode->getName());
  if (afr != asteroidNodes.end()) {

    AsteroidNode* an = afr->second;

    if (an->getType() == RT_big) {
      addTwoAsteroids(an->getAngle(), an->getPosition(), RT_medium);
    }
    else if (an->getType() == RT_medium) {
      addTwoAsteroids(an->getAngle(), an->getPosition(), RT_small);
    }

    asteroidNodes.erase(afr);
  }
  asteroidNode->removeFromParentAndCleanup(true);

  if ( asteroidNodes.empty() ) {
    processGameOver(true);
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .