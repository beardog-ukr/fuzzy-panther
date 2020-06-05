#include "AsteroidsScene.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

#include "main_menu/BackgroundNode.h"
#include "asteroids/GameManager.h"
#include "asteroids/ZOrderValues.h"


USING_NS_CC;
using namespace std;

using namespace asteroids;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AsteroidsScene::AsteroidsScene() {
  // ntdh
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AsteroidsScene::~AsteroidsScene() {
  C6_D1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// on "init" you need to initialize your instance
bool AsteroidsScene::init() {
  //////////////////////////////
  // 1. super init first
  if ( !Scene::init() ) {
    return false;
  }

  c6 = make_shared<SixCatsLogger>(SixCatsLogger::Debug);

  if (!initBackground()) {
    return false;
  }

  gameManager = make_unique<asteroids::GameManager>();
  gameManager->setLogger(c6);

  if (!initGameNode()) {
    return false;
  }

  if (!initKeyboardProcessing()) {
    return false;
  }

  gameManager->startGame();

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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool AsteroidsScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(AsteroidsScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void AsteroidsScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode, Event *) {
  C6_D3(c6, "Key '", (int)keyCode, "' was pressed");

  if (gameManager->processKeyCode(keyCode)) {
    return;
  }

  if (EventKeyboard::KeyCode::KEY_X == keyCode) {
    c6->d(__c6_MN__, "Need to get out.");

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
