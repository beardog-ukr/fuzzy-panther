#include "eyes/EyesMainScene.h"
#include "main_menu/BackgroundNode.h"
using namespace eyes;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

#include <cmath>
#include <ctgmath>

enum ZOrderValues {
  kSceneBackgroundZOrder = 10,
  kSpaceShipZOrder,
};

static const double pi = acos(-1);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

EyesMainScene::EyesMainScene() {
  // ntdh
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

EyesMainScene::~EyesMainScene() {
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

double EyesMainScene::calculateAngle(const float xPos, const float yPos,
                                     const Vec2& shipPos) const {
  const double xDiff = shipPos.x - xPos;
  const double yDiff = shipPos.y - yPos;

  double angleR = atan2(yDiff,xDiff);
  double angleDegrees = (180*angleR)/pi;

  return angleDegrees;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene* EyesMainScene::createScene(std::shared_ptr<SixCatsLogger> inC6) {

  EyesMainScene *pRet = new(std::nothrow) EyesMainScene();
  if (pRet == nullptr) {
    return nullptr;
  }

  pRet->setLogger(inC6);

  if (pRet && pRet->init()) {
    pRet->autorelease();
    return pRet;
  }
  else {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool EyesMainScene::init() {
  //////////////////////////////
  // 1. super init first
  if ( !Scene::init() ) {
    return false;
  }

  if (!initBackground()) {
    return false;
  }

  if(!initSpaceShips()) {
    return false;
  }

  if(!initMouseProcessing()) {
    return false;
  }

  if (!initKeyboardProcessing()) {
    C6_D1(c6, "Failed to init kb processing");
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool EyesMainScene::initBackground() {
  const string bFn = "eyes/eyes_background.png";
  BackgroundNode* backgroundNode = BackgroundNode::create(getContentSize(), bFn, c6);
  if (backgroundNode == nullptr) {
    return false;
  }

  backgroundNode->setAnchorPoint(Vec2(0,0));
  backgroundNode->setPosition(0,0);
  addChild(backgroundNode, kSceneBackgroundZOrder);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool EyesMainScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(EyesMainScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool EyesMainScene::initMouseProcessing() {
  EventListenerMouse* sceneMouseListener = EventListenerMouse::create();
  sceneMouseListener->onMouseMove = CC_CALLBACK_1(EyesMainScene::onMouseMove, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneMouseListener, this);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool EyesMainScene::initSpaceShips() {
  const Size cs = getContentSize();

  const string leftFN = "eyes/left_spaceship.png";
  leftShip = Sprite::create(leftFN);
  if (leftShip == nullptr) {
    C6_C2(c6, "Error while loading: ", leftFN);
    return false;
  }

  leftShipPos.x = cs.width/4;
  leftShipPos.y = cs.height/2;
  leftShip->setPosition(leftShipPos);
  addChild(leftShip, kSpaceShipZOrder);

  const string rightFN = "eyes/right_spaceship.png";
  rightShip = Sprite::create(rightFN);
  if (rightShip == nullptr) {
    C6_C2(c6, "Error while loading: ", rightFN);
    return false;
  }

  rightShipPos.x = 3*(cs.width/4);
  rightShipPos.y = cs.height/2;
  rightShip->setPosition(rightShipPos);
  addChild(rightShip, kSpaceShipZOrder);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void EyesMainScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode, Event *) {
  C6_D3(c6, "Key '", (int)keyCode, "' was pressed");

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

void EyesMainScene::onMouseMove(Event *event) {
  EventMouse* e = (EventMouse*)event;

  const float mx = e->getCursorX();
  const float my = e->getCursorY();

  leftShip->setRotation(-1*calculateAngle(mx, my, leftShipPos));
  rightShip->setRotation(-1*calculateAngle(mx, my, rightShipPos));
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
