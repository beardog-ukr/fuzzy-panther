#include "AsteroidsScene.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

#include "main_menu/BackgroundNode.h"
#include "asteroids/GameManager.h"
#include "asteroids/ZOrderValues.h"

// #include "asteroids/AsteroidNode.h"


USING_NS_CC;
using namespace std;

using namespace asteroids;

static const string plistFilename = "asteroids/asteroids.plist";

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AsteroidsScene::AsteroidsScene() {
  // ntdh
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AsteroidsScene::~AsteroidsScene() {
  C6_D1(c6, "here");
  SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(plistFilename);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene* AsteroidsScene::createScene() {
  // printf("%s: here\n", __func__);
  Scene* result = AsteroidsScene::create();
  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// on "init" you need to initialize your instance
bool AsteroidsScene::init() {
  //////////////////////////////
  // 1. super init first
  if ( !Scene::initWithPhysics() ) {
    return false;
  }

  // getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_SHAPE);

  c6 = make_shared<SixCatsLogger>(SixCatsLogger::Debug);

  if (!initSpriteCache()) {
    return false;
  }

  if (!initBackground()) {
    return false;
  }

  gameManager = make_unique<asteroids::GameManager>();
  gameManager->setLogger(c6);

  if (!initGameNode()) {
    return false;
  }

  // if (!initDebugAsteroids()) {
  //   return false;
  // }

  if (!initKeyboardProcessing()) {
    return false;
  }

  auto contactListener = EventListenerPhysicsContact::create();
  contactListener->onContactBegin = CC_CALLBACK_1(AsteroidsScene::onContactBegin, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

  gameManager->startGame();

  // getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_SHAPE);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool AsteroidsScene::initBackground() {
  const string filename = "menu/seamless-1657428_640.jpg";
  BackgroundNode* backgroundNode = BackgroundNode::create(getContentSize(), filename, c6);
  if (backgroundNode == nullptr) {
    return false;
  }

  backgroundNode->setAnchorPoint(Vec2(0,0));
  backgroundNode->setPosition(0,0);
  addChild(backgroundNode, ZO_scene_background);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool AsteroidsScene::initGameNode() {
  Node * gameNode = gameManager->prepareGameNode();
  if (gameNode == nullptr) {
    return false;
  }

  const Vec2 backupAP = gameNode->getAnchorPoint();

  gameNode->setAnchorPoint(Vec2(0.5,0.5));
  const Size cs = getContentSize();
  gameNode->setPosition(cs.width/2, cs.height/2);
  addChild(gameNode, ZO_game_background);

  gameNode->setAnchorPoint(backupAP);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// bool AsteroidsScene::initDebugAsteroids() {
//   AsteroidNode* asteroidNode = AsteroidNode::create(20, AsteroidNode::RT_medium, c6);

//   asteroidNode->setPosition(10,10);
//   addChild(asteroidNode);
//   return true;
// }

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool AsteroidsScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(AsteroidsScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool AsteroidsScene::initSpriteCache() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();

  sfc->addSpriteFramesWithFile(plistFilename);
  if (!sfc->isSpriteFramesWithFileLoaded(plistFilename)) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void AsteroidsScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode, Event *) {
  C6_D3(c6, "Key '", (int)keyCode, "' was pressed");

  if (gameManager->processKeyCode(keyCode)) {
    return;
  }

  if (EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode) {
    Director::getInstance()->popScene();
  }
  else if (EventKeyboard::KeyCode::KEY_X == keyCode) {
    c6->d(__c6_MN__, "Need to get out.");

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool AsteroidsScene::onContactBegin(PhysicsContact& contact) {
  printf("%s: here\n", __func__);

  return gameManager->processContact(contact);

  // Node* nodeA = contact.getShapeA()->getBody()->getNode();
  // Node* nodeB = contact.getShapeB()->getBody()->getNode();

  // bool laserAndAsteroid = (nodeA->getTag()==IT_laser) && (nodeB->getTag()==IT_asteroid);
  // laserAndAsteroid = laserAndAsteroid ||
  //                    ((nodeA->getTag()==IT_asteroid) && (nodeB->getTag()==IT_laser));

  // if (laserAndAsteroid) {
  //   //   printf("%s: node A is green ship\n", __func__);
  //   addSplashAt(nodeA->getPosition(), nodeB->getPosition());
  //   nodeA->removeFromParentAndCleanup(true);
  //   nodeB->removeFromParentAndCleanup(true);


  //   //   greenShip = nullptr;
  //   // }

  //   // if ((nodeB->getTag()==IT_green_ship)) {
  //   //   printf("%s: node B is green ship\n", __func__);
  //   //   nodeB->removeFromParentAndCleanup(true);
  //   //   greenShip = nullptr;
  //   // }
  // }

  // return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
