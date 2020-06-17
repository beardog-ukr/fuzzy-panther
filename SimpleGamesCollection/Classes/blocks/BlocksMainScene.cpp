#include "blocks/BlocksMainScene.h"
#include "blocks/BlocksGameNode.h"
#include "blocks/CommonUtils.h"
#include "main_menu/BackgroundNode.h"
using namespace blocks;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BlocksMainScene::BlocksMainScene() {
  // ntdh
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BlocksMainScene::~BlocksMainScene() {
//  GameStateKeeper::unloadCardsFromCache();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene* BlocksMainScene::createScene(std::shared_ptr<SixCatsLogger> inC6) {

  BlocksMainScene *pRet = new(std::nothrow) BlocksMainScene();
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlocksMainScene::init() {
  //////////////////////////////
  // 1. super init first
  if ( !Scene::init() ) {
    return false;
  }

  if (!initBackground()) {
    return false;
  }

//  if (!initBoard()) {
//    return false;
//  }

//  if (!initMenu()) {
//    return false;
//  }

//  if (!initGameStateKeeper()) {
//    C6_D1(c6, "Failed to create game state keeper");
//    return false;
//  }

  if (!initGameNode()) {
    C6_D1(c6, "Failed to create game node");
    return false;
  }

  if (!initKeyboardProcessing()) {
    C6_D1(c6, "Failed to init kb processing");
    return false;
  }

  startGame();

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlocksMainScene::initBackground() {
  const string bFn = "menu/seamless-1657428_640.jpg";
  BackgroundNode* backgroundNode = BackgroundNode::create(getContentSize(), bFn, c6);
  if (backgroundNode == nullptr) {
    return false;
  }

  backgroundNode->setAnchorPoint(Vec2(0,0));
  backgroundNode->setPosition(0,0);
  addChild(backgroundNode, ZO_scene_background);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlocksMainScene::initGameNode() {
  gameNode = BlocksGameNode::create(c6);

  gameNode->setAnchorPoint(Vec2(0.5, 0.5));

  const Size cs = getContentSize();
  float desiredGameNodeHeight = cs.height*0.9;

  gameNode->setScale(desiredGameNodeHeight / gameNode->getContentSize().height);

  gameNode->setPosition(cs.width/2, cs.height/2);
  addChild(gameNode, ZO_game_background);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlocksMainScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(BlocksMainScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlocksMainScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode, Event *) {
  C6_D3(c6, "Key '", (int)keyCode, "' was pressed");

  if (EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode) {
    Director::getInstance()->popScene();
  }
  else if (EventKeyboard::KeyCode::KEY_LEFT_ARROW == keyCode) {
    gameNode->doMoveFigureLeft();
  }
  else if (EventKeyboard::KeyCode::KEY_RIGHT_ARROW == keyCode) {
    gameNode->doMoveFigureRight();
  }
  else if (EventKeyboard::KeyCode::KEY_UP_ARROW == keyCode) {
    gameNode->doRotateFigure();
  }
  else if (EventKeyboard::KeyCode::KEY_DOWN_ARROW == keyCode) {
    gameNode->doDropFigure();
  }
  else if (EventKeyboard::KeyCode::KEY_X == keyCode) {
    c6->d(__c6_MN__, "Need to get out.");

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlocksMainScene::startGame() {
  doNextTick();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlocksMainScene::doNextTick() {
  C6_D1(c6, "Here");

  float duration = gameNode->prepareNextIteration();
  if (duration == 0) {
    duration = gameNode->prepareNextIteration();
  }

  if (duration ==0) {
    C6_D1(c6, "bad logic");
    return;
  }

  CallFunc *cf = CallFunc::create([this]() {
    this->doNextTick();
  });

  Sequence* seq = Sequence::create(DelayTime::create(duration), cf, nullptr);
  runAction(seq);
}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
