#include "bird/BirdMainScene.h"
#include "bird/GameEndScene.h"
#include "bird/WizardNode.h"

#include "main_menu/BackgroundNode.h"

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

static const int sectionsForVictoryRequirement = 10;


enum ActionTagsBirdMainScene {
  BMSAT_main =1
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BirdMainScene::BirdMainScene() {
  MapSectionNode::loadCached();
  WizardNode::loadAnimations();

  mapSections.resize(mapSectionsCount+2);
  mapSectionPositions.resize(mapSectionsCount+2);

  obstacleLevelGeneratorArr = {8,7,8,10,11, 9};
  nextObstacleLevelGeneratorIdx = 0;

  sectionsPassedCounter = 0;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BirdMainScene::~BirdMainScene() {
  MapSectionNode::unloadCached();
  WizardNode::unloadAnimations();
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

void BirdMainScene::doOneTick() {
  sectionsPassedCounter++;
  if (sectionsPassedCounter >= sectionsForVictoryRequirement) {
    stopBeforeGameOver();
    resetAfterGameOver(true);
    return;
  }

  if (mapSections[0] != nullptr) {
    C6_D1(c6, "Calling remove for some section");
    mapSections[0]->removeFromParentAndCleanup(true);
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
  if ( !Scene::initWithPhysics() ) {
    return false;
  }

  // getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_SHAPE);

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

  auto contactListener = EventListenerPhysicsContact::create();
  contactListener->onContactBegin = CC_CALLBACK_1(BirdMainScene::onContactBegin, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

  startGame();

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BirdMainScene::initBackground() {

  if (!initBackgroundBorder()) {
    return false;
  }

  if (!initBackgroundGame()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BirdMainScene::initBackgroundBorder() {
  const string bFn = "menu/seamless-1657428_640.jpg";
  BackgroundNode* backgroundSource = BackgroundNode::create(getContentSize(), bFn, c6);
  if (backgroundSource == nullptr) {
    return false;
  }

  backgroundSource->setAnchorPoint(Vec2(0,0));
  backgroundSource->setPosition(0,0);
  addChild(backgroundSource);

  // const string bFn = "bird/full_scene.png";
  // Sprite* backgroundSource = Sprite::create(bFn);
  // if (backgroundSource == nullptr) {
  //   C6_C2(c6, "Error while loading: ", bFn);
  //   return false;
  // }
  // backgroundSource->setAnchorPoint(Vec2(0,0));
  // backgroundSource->setPosition(0, 0);
  // addChild(backgroundSource, ZO_scene_background);

  const Size cs = getContentSize();
  const int rtH = (int)round(cs.height);
  const int rtW = (int)round(cs.width/2 - (gameWindowWidth/2));

  RenderTexture* rtLeft = RenderTexture::create(rtW, rtH);
  rtLeft->begin();
  backgroundSource->visit();
  rtLeft->end();
  Director::getInstance()->getRenderer()->render();

  rtLeft->setPosition(rtW/2,rtH/2);
  addChild(rtLeft, ZO_scene_border);

  RenderTexture* rtUp = RenderTexture::create(gameWindowWidth, (cs.height-gameWindowHeight)/2);

  backgroundSource->setPosition(0 - (cs.width/2 - (gameWindowWidth/2)),
                                0-(gameWindowHeight + (cs.height-gameWindowHeight)/2));

  rtUp->begin();
  backgroundSource->visit();
  rtUp->end();
  Director::getInstance()->getRenderer()->render();

  rtUp->setPosition(cs.width/2,
                    gameWindowHeight + (cs.height-gameWindowHeight)/2 +
                    (cs.height-gameWindowHeight)/4);
  addChild(rtUp, ZO_scene_border);

  RenderTexture* rtDown = RenderTexture::create(gameWindowWidth, (cs.height-gameWindowHeight)/2);

  backgroundSource->setPosition(0 - (cs.width/2 - (gameWindowWidth/2)), 0);

  rtDown->begin();
  backgroundSource->visit();
  rtDown->end();
  Director::getInstance()->getRenderer()->render();

  rtDown->setPosition(cs.width/2,(cs.height-gameWindowHeight)/4);
  addChild(rtDown, ZO_scene_border);

  RenderTexture* rtRight = RenderTexture::create(rtW, rtH);

  backgroundSource->setPosition(0 - cs.width/2 - (gameWindowWidth/2), 0);

  rtRight->begin();
  backgroundSource->visit();
  rtRight->end();
  Director::getInstance()->getRenderer()->render();

  rtRight->setPosition(cs.width - rtW/2,rtH/2);
  addChild(rtRight, ZO_scene_border);


  // --- finally remove background, it's not needed anymore
  backgroundSource->removeFromParent();

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BirdMainScene::initBackgroundGame() {

  const char filename[] = "bird/map_background.png";
  mapBackgroundSprite = Sprite::create(filename);
  if (mapBackgroundSprite == nullptr) {
    C6_C2(c6, "Error while loading: ", filename);
    return false;
  }

  mapBackgroundSprite->setAnchorPoint(Vec2(0,0));
  resetGameBackgroundPosition();

  addChild(mapBackgroundSprite, ZO_scene_background);

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
  wizardNode = WizardNode::create(c6);
  if (wizardNode==nullptr) {
    return false;
  }

  const Size sceneSize = getContentSize();
  Vec2 wizardBasePosition;
  wizardBasePosition.x = sceneSize.width/2 - gameWindowWidth/2 + mapSectionWidth/2;
  wizardBasePosition.y = sceneSize.height/2;

  wizardNode->setPosition(wizardBasePosition);
  wizardNode->setLowPoint({.x = wizardBasePosition.x,
                           .y = sceneSize.height/2 - gameWindowHeight/2});
  addChild(wizardNode, ZO_wizard);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BirdMainScene::onContactBegin(PhysicsContact& contact) {
  stopBeforeGameOver();

  CallFunc *cf = CallFunc::create([this]() {
    this->resetAfterGameOver(false);
  });

  wizardNode->doDie(cf);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BirdMainScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode, Event *) {
  C6_D3(c6, "Key '", (int)keyCode, "' was pressed");

  if (EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode) {
    Director::getInstance()->popScene();
  }
  else if (EventKeyboard::KeyCode::KEY_SPACE == keyCode) {
    wizardNode->doGoUp();
  }
  else if (EventKeyboard::KeyCode::KEY_X == keyCode) {
    c6->d(__c6_MN__, "Need to get out.");

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BirdMainScene::resetAfterGameOver(const bool gameResult) {
  // --- show game end scene
  Scene* newScene = bird::GameEndScene::createScene(gameResult, c6);
  if (newScene!= nullptr) {
    Director::getInstance()->pushScene(newScene);
  }

  // --- delete active elements of the current scene
  //note, last element of mapSections array is not used
  for (int i = 0; i<(mapSectionsCount+1); i++) {
    if (mapSections[i]==nullptr) {
      continue;
    }
    mapSections[i]->removeFromParentAndCleanup(true);
    mapSections[i] = nullptr;
  }

  wizardNode->removeFromParentAndCleanup(true);
  nextObstacleLevelGeneratorIdx = 0;
  sectionsPassedCounter = 0;
  resetGameBackgroundPosition();

  // --- sceate active elements again and start
  initModules();
  initWizard();
  startGame();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BirdMainScene::resetGameBackgroundPosition() {
  const Size cs = getContentSize();
  Vec2 basePos;
  basePos.x = cs.width/2 - gameWindowWidth/2;
  basePos.y = (cs.height - gameWindowHeight)/2;

  mapBackgroundSprite->setPosition(basePos);  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BirdMainScene::startGame() {
  CallFunc *cf = CallFunc::create([this]() {
    this->doOneTick();
  });
  Sequence* seq = Sequence::create(cf, DelayTime::create(mapSectionMoveDuration), nullptr);
  Repeat* ra = Repeat::create(seq, 20);
  ra->setTag(BMSAT_main);
  runAction(ra);

  wizardNode->start();//
  startMoveGameBackground();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BirdMainScene::startMoveGameBackground() {
  const Size mgbSize = mapBackgroundSprite->getContentSize();

  const float duration = sectionsForVictoryRequirement*mapSectionMoveDuration;
  MoveBy* mba = MoveBy::create(duration, Vec2(0 - (mgbSize.width - gameWindowWidth), 0));

  mapBackgroundSprite->runAction(mba);
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BirdMainScene::stopBeforeGameOver() {
  for (int i = 0; i<(mapSectionsCount+2); i++) {
    if (mapSections[i]==nullptr) {
      continue;
    }
    mapSections[i]->stopAllActions();
  }

  stopAllActionsByTag(BMSAT_main);
  mapBackgroundSprite->stopAllActions();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
