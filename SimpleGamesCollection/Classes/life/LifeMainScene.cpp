#include "life/LifeMainScene.h"

#include "life/GameNode.h"
#include "life/ZOrderValues.h"
// #include "main_menu/BackgroundNode.h"

using namespace life;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

LifeMainScene::LifeMainScene() {

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

LifeMainScene::~LifeMainScene() {

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene* LifeMainScene::createScene(std::shared_ptr<SixCatsLogger> inC6) {

  LifeMainScene *pRet = new(std::nothrow) LifeMainScene();
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

bool LifeMainScene::init() {
  //////////////////////////////
  // 1. super init first
  if ( !Scene::init() ) {
    return false;
  }

  cameraIsMoving = false;

  if (!initGameNode()) {
    return false;
  }

  if (!initMouseProcessing()) {
    return false;
  }

  if (!initKeyboardProcessing()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool LifeMainScene::initGameNode() {
  gameNode = GameNode::create(c6);

  gameNode->setAnchorPoint(Vec2(0, 0));

  // const Size cs = getContentSize();
  mapSize = gameNode->getContentSize();

  gameNode->setPosition(0, 0);
  addChild(gameNode, kGameBackgroundZOrder);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool LifeMainScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(LifeMainScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool LifeMainScene::initMouseProcessing() {
  EventListenerMouse* sceneMouseListener = EventListenerMouse::create();
  sceneMouseListener->onMouseUp = CC_CALLBACK_1(LifeMainScene::onMouseUp, this);
  sceneMouseListener->onMouseDown = CC_CALLBACK_1(LifeMainScene::onMouseDown, this);
  sceneMouseListener->onMouseMove = CC_CALLBACK_1(LifeMainScene::onMouseMove, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneMouseListener, this);

  mouseButtonPressed = 0;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void LifeMainScene::moveCamera(const int xDiff, const int yDiff) {
  if (cameraIsMoving) {
    return;
  }

  Camera* camera = getDefaultCamera();

  Vec3 currentPos = camera->getPosition3D();
  Vec3 newPos = Vec3(currentPos.x + xDiff, currentPos.y + yDiff, currentPos.z);

  Size winSize = Director::getInstance()->getWinSize();
  const float xBorderLimit = winSize.width/2;
  if (mapSize.width - newPos.x < xBorderLimit) {
    newPos.x = mapSize.width - xBorderLimit;
  }
  if (newPos.x < xBorderLimit) {
    newPos.x = xBorderLimit;
  }

  const float yBorderLimit = winSize.height/2;
  if (mapSize.height - newPos.y < yBorderLimit) {
    newPos.y = mapSize.height - yBorderLimit;
  }
  if (newPos.y < yBorderLimit) {
    newPos.y = yBorderLimit;
  }


  MoveTo* mt = MoveTo::create(1, newPos);
  cameraIsMoving = true;

  CallFunc *cf = CallFunc::create([this]() {
    this->cameraIsMoving = false;
  });

  camera->runAction(Sequence::create(mt, cf, nullptr));
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void LifeMainScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode, Event *) {
  C6_D3(c6, "Key '", (int)keyCode, "' was pressed");

  if (EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode) {
    Director::getInstance()->popScene();
  }
  else if (EventKeyboard::KeyCode::KEY_SPACE == keyCode) {
    gameNode->applyChanges();
  }
  else if (EventKeyboard::KeyCode::KEY_LEFT_ARROW == keyCode) {
    moveCamera(-100, 0);
  }
  else if (EventKeyboard::KeyCode::KEY_RIGHT_ARROW == keyCode) {
    moveCamera(100, 0);
  }
  else if (EventKeyboard::KeyCode::KEY_UP_ARROW == keyCode) {
    moveCamera(0, 100);
  }
  else if (EventKeyboard::KeyCode::KEY_DOWN_ARROW == keyCode) {
    moveCamera(0, -100);
  }
  else if (EventKeyboard::KeyCode::KEY_X == keyCode) {
    c6->d(__c6_MN__, "Need to get out.");

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void LifeMainScene::onMouseDown(Event *event) {
  if (cameraIsMoving) {
    return;
  }

  mouseButtonPressed++;

  EventMouse* eventMouse = (EventMouse*)event;
  currentActiveMouseButton = eventMouse->getMouseButton();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void LifeMainScene::onMouseMove(Event *event) {
  if (cameraIsMoving) {
    return;
  }

  if (mouseButtonPressed <=0) {
    return;
  }

  const Vec3 currentCameraPos = getDefaultCamera()->getPosition3D();
  const Size cs = getContentSize();
  Vec2 basePos = {
    .x = currentCameraPos.x - (cs.width/2),
    .y = currentCameraPos.y - (cs.height/2)
  };

  EventMouse* eventMouse = (EventMouse*)event;

  float xPos = basePos.x + eventMouse->getCursorX();
  float yPos = basePos.y + eventMouse->getCursorY();
  C6_T4(c6,"Mouse Down detected, Position ", xPos, ":", yPos);

  const float xRel = xPos/mapSize.width;
  const float yRel = yPos/mapSize.height;

  gameNode->processClick(xRel, yRel, currentActiveMouseButton );
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void LifeMainScene::onMouseUp(Event *event) {
  if (cameraIsMoving) {
    return;
  }

  mouseButtonPressed--;

  EventMouse* eventMouse = (EventMouse*)event;
  currentActiveMouseButton = eventMouse->getMouseButton();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
