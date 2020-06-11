#include "bird/BirdMainScene.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;
#include <cmath>

#include "bird/CommonUtils.h"
#include "bird/MapSectionNode.h"
using namespace bird;

static const int gameWindowWidth = 640;
static const int gameWindowHeight = 576;

static const int mapSectionWidth = 192;
static const int mapSectionHeight = 576;

static const float mapSectionMoveDuration = 3.0;
static const int mapSectionsCount = 4;



// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BirdMainScene::BirdMainScene() {
  mapSections.resize(mapSectionsCount+2);
  mapSectionPositions.resize(mapSectionsCount+2);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BirdMainScene::~BirdMainScene() {
  C6_D1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

MapSectionNode* BirdMainScene::addNewMapSection() {
  MapSectionNode* msNode = MapSectionNode::create("bird/module01.png", c6);
  if (msNode == nullptr) {
    return nullptr;
  }

  const Size sceneSize = getContentSize();
// Vec2 mp;
// mp.x = sceneSize.width/2 - gameWindowWidth/2 + moduleWidth*(3+1) - moduleWidth/2;
// mp.y = sceneSize.height/2;
  msNode->setPosition(mapSectionPositions[mapSectionsCount+1]);
  addChild(msNode, ZO_game_background);

  // mapSections[mapSectionsCount+1] = msNode;

  // MoveTo* mta = MoveTo::create(moduleMoveDuration*(3+1), pointModuleDelete);
  // CallFunc *cf = CallFunc::create([this]() {
  //   this->addNewModule();
  // });
  // Sequence* seq = Sequence::create(mta, cf, RemoveSelf::create(), nullptr);
  // moduleNode->runAction(seq);

  return msNode;
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

  startGame();

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
  const string msNames[mapSectionsCount] = {
    "bird/module01.png", "bird/module02.png", "bird/module03.png", "bird/module04.png"
  };

  const Size sceneSize = getContentSize();

  // 0 is for point where section gets deleted
  mapSectionPositions[0].x = round(sceneSize.width/2 - gameWindowWidth/2 - mapSectionWidth/2);
  mapSectionPositions[0].y = sceneSize.height/2;

  for (int i = 1; i<(mapSectionsCount+2); i++) {
    mapSectionPositions[i].x = mapSectionPositions[i-1].x + mapSectionWidth;
    mapSectionPositions[i].y = mapSectionPositions[0].y;
  }

  mapSections[0] = nullptr;
  for (int i = 0; i<mapSectionsCount; i++) {
    MapSectionNode* msNode = MapSectionNode::create(msNames[i], c6);
    if (msNode == nullptr) {
      return false;
    }

    msNode->setPosition(mapSectionPositions[i+1]);
    addChild(msNode, ZO_game_background);

    mapSections[i+1] = msNode;



    // MoveTo* mta;
    // if (i>0) {
    //   mta = MoveTo::create(moduleMoveDuration, points[i-1]);
    // }
    // else {
    //   mta = MoveTo::create(moduleMoveDuration, pointModuleDelete);//
    // }
    // // CallFunc *cf = CallFunc::create([this]() {
    // // this->addNewModule();
    // // });
    // //Sequence* seq = Sequence::create(mta, cf, RemoveSelf::create(), nullptr);
    // Sequence* seq = Sequence::create(mta, RemoveSelf::create(), nullptr);
    // moduleNode->runAction(seq);
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
  wizardBasePosition.x = sceneSize.width/2 - gameWindowWidth/2 + mapSectionWidth/2;
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
  else if (EventKeyboard::KeyCode::KEY_SPACE == keyCode) {
    doOneTick();
  }
  else if (EventKeyboard::KeyCode::KEY_X == keyCode) {
    c6->d(__c6_MN__, "Need to get out.");

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BirdMainScene::startGame() {

  CallFunc *cf = CallFunc::create([this]() {
    this->doOneTick();
  });

  Sequence* seq = Sequence::create(cf, DelayTime::create(mapSectionMoveDuration), nullptr);

  Repeat* ra = Repeat::create(seq, 20);

  runAction(ra);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BirdMainScene::doOneTick() {

  if (mapSections[0] != nullptr) {
    mapSections[0]->removeFromParentAndCleanup(true);
    mapSections[0] = nullptr;
  }

  mapSections[mapSectionsCount+1] = addNewMapSection();

  for (int i = 1; i<(mapSectionsCount+2); i++) {
    if (mapSections[i]==nullptr) {
      continue;
    }
    mapSections[i]->stopAllActions();
    MoveTo* mta = MoveTo::create(mapSectionMoveDuration, mapSectionPositions[i-1]);
    mapSections[i]->runAction(mta);
    mapSections[i-1] = mapSections[i];
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
