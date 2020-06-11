#include "bird/BirdMainScene.h"
#include "bird/WizardNode.h"

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

  obstacleLevelGeneratorArr = {8,7,8,9};
  nextObstacleLevelGeneratorIdx = 0;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BirdMainScene::~BirdMainScene() {
  C6_D1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

MapSectionNode* BirdMainScene::addNewMapSection(const Vec2& position) {
  int nol = getNextObstacleLevel();

  MapSectionNode* msNode = MapSectionNode::create(nol, c6);
  if (msNode == nullptr) {
    return nullptr;
  }

  msNode->setPosition(position);
  addChild(msNode, ZO_game_background);

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

int BirdMainScene::getNextObstacleLevel() {
  int result = obstacleLevelGeneratorArr[nextObstacleLevelGeneratorIdx];
  nextObstacleLevelGeneratorIdx++;
  if (nextObstacleLevelGeneratorIdx >= obstacleLevelGeneratorArr.size()) {
    nextObstacleLevelGeneratorIdx = 0;
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BirdMainScene::init() {
  //////////////////////////////
  // 1. super init first
  // if ( !Scene::init() ) {
  //   return false;
  // }
  if ( !Scene::initWithPhysics() ) {
    return false;
  }

  getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_SHAPE);


  // if (!initBackground()) {
  //   return false;
  // }

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
    mapSections[i+1] = addNewMapSection(mapSectionPositions[i+1]);
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BirdMainScene::initWizard() {
  // const string bFn = "bird/wizard_idle_00.png";
  // Sprite* sprite;
  // sprite = Sprite::create(bFn);
  // if (sprite == nullptr) {
  //   C6_C2(c6, "Error while loading: ", bFn);
  //   return false;
  // }

  wizardNode = WizardNode::create(c6);
  if (wizardNode==nullptr) {
    return false;
  }

  const Size sceneSize = getContentSize();
  Vec2 wizardBasePosition;
  wizardBasePosition.x = sceneSize.width/2 - gameWindowWidth/2 + mapSectionWidth/2;
  wizardBasePosition.y = sceneSize.height/2;

  wizardNode->setPosition(wizardBasePosition);
  addChild(wizardNode, ZO_wizard);

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
    C6_D1(c6, "Calling remove for some section");
    mapSections[0]->removeFromParentAndCleanup(true);
    // mapSections[0]->release();

    mapSections[0] = nullptr;
  }

  mapSections[mapSectionsCount+1] = addNewMapSection(mapSectionPositions[mapSectionsCount+1]);

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
