#include "snake/GameNode.h"
#include "snake/BodyPartsFactory.h"
#include "snake/GameStateKeeper.h"
#include "snake/ZOrderValues.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace snake;

// const int GameNode::kKeysCount = 4;

static const string kPlistFileName = "snake/snake_images.plist";
static const string kAnimationsPlistFileName = "snake/snake_animations.plist";

static const string kBarrelIdleAnimationName = "barrel_idle";
static const string kBarrelDieAnimationName = "barrel_die";

static const float kIntervalDuration = 2.0;
// static const float kTimeShowOneDigit = 3.0;
// static const float kTimeHideWhiteRocket = 1.0;

const int GameNode::kCellSize = 80;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameNode::GameNode() {
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameNode::~GameNode() {
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameNode* GameNode::create(std::shared_ptr<SixCatsLogger> inc6) {
  GameNode *result = new (std::nothrow) GameNode();

  if (result == nullptr) {
    return nullptr;
  }

  result->setLogger(inc6);

  if (!result->initSelf()) {
    CC_SAFE_DELETE(result);
    return nullptr;
  }

  result->autorelease();
  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::doOneTick(float) {
  C6_D1(c6, "here ");

  for (Node* sn:previousSnakeNodes) {
    sn->removeFromParentAndCleanup(true);
  }
  previousSnakeNodes.clear();

  const MoveResultType moveResult = gameStateKeeper->performNextMove();
  if (moveResult == kGameFailure) {
    C6_D1(c6, "wooops, game over ");
    unschedule(CC_SCHEDULE_SELECTOR(GameNode::doOneTick));
  }

  processBarrel(moveResult);

  Sprite*(BodyPartsFactory::*partCreateFunc)(const SnakeElementInfo& ) = nullptr;
  if (moveResult == kSimpleMove) {
    partCreateFunc = &BodyPartsFactory::createMovingPart;
  }
  else if (moveResult == kIdleConsume) {
    partCreateFunc = &BodyPartsFactory::createIdlePart;
  }
  else {
    C6_D1(c6, "Game failed, no work on nodes");
    sceneCallback(false);
    return;
  }

  list<SnakeElementInfo> snakePartsInfo = gameStateKeeper->prepareSnakeInfo();

  for (const SnakeElementInfo& sbInfo: snakePartsInfo) {
    Sprite* sp = (*bodyPartsFactory.*partCreateFunc)(sbInfo);
    if (sp == nullptr) {
      C6_D1(c6, "Failed to create sprite");
      continue;
    }

    addChild(sp, kSnakeZOrder);
    previousSnakeNodes.push_back(sp);    //
  }


}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::initSelf() {
  const string bgfn = "snake/snake_background.png";
  if (!initWithFile(bgfn)) {
    C6_C2(c6, "Failed to init with file ", bgfn);
    return false;    //
  }

  gameStateKeeper = make_unique<GameStateKeeper>();
  gameStateKeeper->setLogger(c6);

  bodyPartsFactory = make_unique<BodyPartsFactory>();
  bodyPartsFactory->setLogger(c6);

//  currentBarrelGameX = -1;
//  currentBarrelGameY = -1;
  currentBarrelNode = nullptr;

  // whiteRocket = nullptr;

  // if (!initDigitNodes()) {
  //   return false;
  // }

  // if (!initWhiteRocket()) {
  //   return false;
  // }


//  startGame();

  C6_D1(c6, "d2");

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// bool GameNode::initWhiteRocket() {
//   const string fn = "repeat/white_rocket/white_rocket_10.png";
//   whiteRocket = Sprite::createWithSpriteFrameName(fn);
//   if (whiteRocket == nullptr) {
//     C6_C2(c6, "Failed to init with ", fn);
//     return false;      //
//   }


//   whiteRocket->setVisible(false);

//   addChild(whiteRocket, kFireworksZOrder);

//   return true;
// }

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::loadSpriteCache(std::shared_ptr<SixCatsLogger> c6) {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();

  sfc->addSpriteFramesWithFile(kPlistFileName);
  if (!sfc->isSpriteFramesWithFileLoaded(kPlistFileName)) {
    C6_C2(c6, "Failed to find ", kPlistFileName);
    return false;
  }

  AnimationCache * const ac = AnimationCache::getInstance();
  ac->addAnimationsWithFile(kAnimationsPlistFileName);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::processBarrel(const MoveResultType moveResult) {
  const SnakeElementInfo nbp = gameStateKeeper->getBarrelInfo();
//  if ((nbp.gameX == currentBarrelGameX)&&(nbp.gameY == currentBarrelGameY)) {
//    if (moveResult != kIdleConsume) {
//      return;
//    }
//  }

  if (currentBarrelNode) {
    currentBarrelNode->removeFromParentAndCleanup(true);
  }

//  currentBarrelGameX = nbp.gameX;
//  currentBarrelGameY = nbp.gameY;
//  C6_D4(c6, "Barrel Game Pos is  ", currentBarrelGameX, ":", currentBarrelGameY);

  if ((moveResult == kSimpleMove)||(moveResult == kGameFailure)) {
    currentBarrelNode = bodyPartsFactory->createBarrel(nbp.targetX, nbp.targetY);
  }
  else {
    currentBarrelNode = bodyPartsFactory->createBarrelExplode(nbp.targetX, nbp.targetY);
  }
  addChild(currentBarrelNode, kSnakeZOrder);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::processKey(cocos2d::EventKeyboard::KeyCode keyCode) {
  MoveType mt;
  if (EventKeyboard::KeyCode::KEY_LEFT_ARROW == keyCode) {
    mt = kMoveWest;
  }
  else if (EventKeyboard::KeyCode::KEY_RIGHT_ARROW == keyCode) {
    mt = kMoveEast;
  }
  else if (EventKeyboard::KeyCode::KEY_UP_ARROW == keyCode) {
    mt = kMoveNorth;
  }
  else if (EventKeyboard::KeyCode::KEY_DOWN_ARROW == keyCode) {
    mt = kMoveSouth;
  }
  else {
    return;
  }

  gameStateKeeper->setNextMove(mt);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::startNewGame(SceneCallback sc) {
  sceneCallback = sc;

  gameStateKeeper->reset();

  C6_D1(c6, "here ");
  doOneTick(0);
  schedule(CC_SCHEDULE_SELECTOR(GameNode::doOneTick), kIntervalDuration, CC_REPEAT_FOREVER, 0);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::startGame() {
  gameStateKeeper->reset();

  C6_D1(c6, "here ");
  doOneTick(0);
  schedule(CC_SCHEDULE_SELECTOR(GameNode::doOneTick), kIntervalDuration, CC_REPEAT_FOREVER, 0);
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::unloadSpriteCache() {
  AnimationCache::getInstance()->removeAnimation(kBarrelDieAnimationName);
  AnimationCache::getInstance()->removeAnimation(kBarrelIdleAnimationName);

  SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(kPlistFileName);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
