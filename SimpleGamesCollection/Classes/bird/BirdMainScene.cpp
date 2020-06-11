#include "bird/BirdMainScene.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

#include "bird/CommonUtils.h"
#include "bird/ModuleNode.h"
using namespace bird;

static const int gameWindowWidth = 640;
static const int gameWindowHeight = 576;

static const int moduleWidth = 192;
static const int moduleHeight = 576;

static const float moduleMoveDuration = 3.0;



// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BirdMainScene::BirdMainScene() {
  // ntdh
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BirdMainScene::~BirdMainScene() {
  C6_D1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BirdMainScene::addNewModule() {
  ModuleNode* moduleNode = ModuleNode::create("bird/module01.png", c6);
  if (moduleNode == nullptr) {
    return;
  }

  const Size sceneSize = getContentSize();
  Vec2 mp;
  mp.x = sceneSize.width/2 - gameWindowWidth/2 + moduleWidth*(3+1) - moduleWidth/2;
  mp.y = sceneSize.height/2;
  moduleNode->setPosition(mp);
  addChild(moduleNode, ZO_game_background);

  MoveTo* mta = MoveTo::create(moduleMoveDuration*(3+1), pointDelete);
  CallFunc *cf = CallFunc::create([this]() {
    this->addNewModule();
  });
  Sequence* seq = Sequence::create(mta, cf, RemoveSelf::create(), nullptr);
  moduleNode->runAction(seq);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene* BirdMainScene::createScene(std::shared_ptr<SixCatsLogger> inC6) {

  BirdMainScene *pRet = new(std::nothrow) BirdMainScene();
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

bool BirdMainScene::init() {
  //////////////////////////////
  // 1. super init first
  if ( !Scene::init() ) {
    return false;
  }

  if (!initBackground()) {
    return false;
  }

  if (!initModules()) {
    return false;
  }

  if (!initWizard()) {
    return false;
  }

  if (!initKeyboardProcessing()) {
    return false;
  }
  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BirdMainScene::initBackground() {
  const string bFn = "bird/full_scene.png";
  Sprite* sprite;
  sprite = Sprite::create(bFn);
  if (sprite == nullptr) {
    C6_C2(c6, "Error while loading: ", bFn);
    return false;
  }
  sprite->setAnchorPoint(Vec2(0,0));
  sprite->setPosition(0, 0);
  addChild(sprite, ZO_scene_background);


  const char filename[] = "bird/visible_area.png";
  sprite = Sprite::create(filename);
  if (sprite == nullptr) {
    C6_C2(c6, "Error while loading: ", filename);
    return false;
  }

  sprite->setAnchorPoint(Vec2(0.5,0.5));
  const Size cs = getContentSize();
  sprite->setPosition(cs.width/2, cs.height/2);
  addChild(sprite, ZO_scene_background);

  return true;
}


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BirdMainScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(BirdMainScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BirdMainScene::initModules() {
  const int amCount = 4;
  const string moduleNames[amCount] = {
    "bird/module01.png", "bird/module02.png", "bird/module03.png", "bird/module04.png"
  };
  // const ModuleNode*

  const Size sceneSize = getContentSize();

  pointDelete.x = sceneSize.width/2 - gameWindowWidth/2 - moduleWidth/2;
  pointDelete.y = sceneSize.height/2;

  for (int i = 0; i<amCount; i++) {
    ModuleNode* moduleNode = ModuleNode::create(moduleNames[i], c6);
    if (moduleNode == nullptr) {
      return false;
    }
    Vec2 mp;
    mp.x = sceneSize.width/2 - gameWindowWidth/2 + moduleWidth*(i+1) - moduleWidth/2;
    mp.y = sceneSize.height/2;
    moduleNode->setPosition(mp);
    addChild(moduleNode, ZO_game_background);

    MoveTo* mta = MoveTo::create(moduleMoveDuration*(i+1), pointDelete);
    CallFunc *cf = CallFunc::create([this]() {
      this->addNewModule();
    });
    Sequence* seq = Sequence::create(mta, cf, RemoveSelf::create(), nullptr);
    moduleNode->runAction(seq);
  }



  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BirdMainScene::initWizard() {
  const string bFn = "bird/wizard_idle_00.png";
  Sprite* sprite;
  sprite = Sprite::create(bFn);
  if (sprite == nullptr) {
    C6_C2(c6, "Error while loading: ", bFn);
    return false;
  }

  const Size sceneSize = getContentSize();
  Vec2 wizardBasePosition;
  wizardBasePosition.x = sceneSize.width/2 - gameWindowWidth/2 + moduleWidth/2;
  wizardBasePosition.y = sceneSize.height/2;

  sprite->setPosition(wizardBasePosition);
  addChild(sprite, ZO_wizard);



  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BirdMainScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode, Event *) {
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
