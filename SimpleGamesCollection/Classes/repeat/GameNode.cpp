#include "repeat/GameNode.h"
#include "repeat/DigitNode.h"
#include "repeat/ZOrderValues.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace repeat;

const int GameNode::kKeysCount = 4;

static const string kPlistFileName = "repeat/repeat.plist";
static const string kAnimationsPlistFileName = "repeat/animations.plist";

static const string kWhiteRocketAnimationName = "white_rocket";

static const float kTimeWaitOneDigit = 3.0;
static const float kTimeShowOneDigit = 3.0;
static const float kTimeHideWhiteRocket = 1.0;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameNode::GameNode() {
  gameFailed = false;
  sceneCallback = nullptr;
  isWaitingForKey = false;

  currentIteration = 1;
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

void GameNode::doNextIteration() {
  isWaitingForKey = false;
  currentIteration++;
  if (currentIteration> iterationsCount) {
    sceneCallback(true);
    return;
  }

  seqElementsCount = currentIteration;
  currentSeqElemIdx = 0;
  launchShow();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::getGameResult() const {
  return (!gameFailed);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::initDigitNodes() {
  const Size cs = getContentSize();
  float xStep = cs.width/kKeysCount;
  float xAcc = xStep/2;
  float yBase = cs.height - (cs.height/4);

  digitNodes.resize(kKeysCount);

  for (int i = 0; i<kKeysCount; i++) {
    DigitNode* dn = DigitNode::create(i+1,c6);
    dn->setPosition(Vec2(xAcc, yBase));
    xAcc += xStep;

    addChild(dn, kDigitsZOrder);
    digitNodes[i] = dn;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::initSelf() {
  const string bgfn = "repeat/repeat_green_background.png";
  if (!initWithFile(bgfn)) {
    C6_C2(c6, "Failed to init with file ", bgfn);
    return false;    //
  }

  whiteRocket = nullptr;

  if (!initDigitNodes()) {
    return false;
  }

  if (!initWhiteRocket()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::initWhiteRocket() {
  const string fn = "repeat/white_rocket/white_rocket_10.png";
  whiteRocket = Sprite::createWithSpriteFrameName(fn);
  if (whiteRocket == nullptr) {
    C6_C2(c6, "Failed to init with ", fn);
    return false;      //
  }


  whiteRocket->setVisible(false);

  addChild(whiteRocket, kFireworksZOrder);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::launchShow() {
  digitNodes[gameSeq[0]-1]->showSelf(kTimeShowOneDigit);

  Vector< FiniteTimeAction * > actions;

  DelayTime* dt = DelayTime::create(kTimeShowOneDigit);
  actions.pushBack(dt);

  for (int i = 1; i<seqElementsCount; i++) {
    CallFunc* cf = CallFunc::create([this]() {
      this->showNextDigit();
    });
    actions.pushBack(cf);

    DelayTime* dt = DelayTime::create(kTimeShowOneDigit);
    actions.pushBack(dt);
  }

  CallFunc* cf = CallFunc::create([this]() {
    this->startWaiting();
  });
  actions.pushBack(cf);

  runAction(Sequence::create(actions));
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::launchWhiteRocket() {
  const Size cs = getContentSize();

  Vec2 wrp; // white rocket pos
  wrp.x = (cs.width/kKeysCount)/2 + (cs.width/kKeysCount)*(currentSeqElemIdx%4 +1);
  wrp.y = cs.height/8;

  whiteRocket->setPosition(wrp);
  whiteRocket->setVisible(true);

  const Vec2 wrnp = {.x = wrp.x, .y = cs.height - cs.height/8};// white rocket next pos

  MoveTo* mt = MoveTo::create(kTimeWaitOneDigit, wrnp);
  CallFunc* cf = CallFunc::create([this]() {
    this->processWaitingOver();

  });

  Sequence* seq = Sequence::create(mt, cf, nullptr);

  whiteRocket->runAction(seq);

  FadeIn* fin = FadeIn::create(kTimeHideWhiteRocket/2);
  whiteRocket->runAction(fin);

  Animation* animation = AnimationCache::getInstance()->getAnimation(kWhiteRocketAnimationName);
  Animate* animate = Animate::create(animation);
  whiteRocket->runAction(animate);
}

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

void GameNode::prepareGameSeq() {
  gameSeq.resize(iterationsCount);
//  gameSeq = {1,2,4,3,2,2,3,3};
  gameSeq[0] = RandomHelper::random_int((int)1, 4);
  for(int i = 1; i< iterationsCount; i++) {
    int rv = RandomHelper::random_int((int)1, 4);
    if (rv == gameSeq[i-1]) {
      rv++;
      if (rv>4) {
        rv = 1;
      }
    }
    gameSeq[i] = rv;
  }

  for(const int& v:gameSeq ) {
    C6_D2(c6, "Added value to seq: ", v);
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::processKey(cocos2d::EventKeyboard::KeyCode keyCode) {
  if (!isWaitingForKey) {
    C6_D2(c6, "Skipped, is not waiting now", (int)keyCode);
    return;
  }

  // stop the rocket;
  whiteRocket->stopAllActions();

  //check if key is correct
  bool guessed = false;
  if (keyCode == EventKeyboard::KeyCode::KEY_1) {
    guessed = (gameSeq[currentSeqElemIdx] == 1);
  }
  else if (keyCode == EventKeyboard::KeyCode::KEY_2) {
    guessed = (gameSeq[currentSeqElemIdx] == 2);
  }
  else if (keyCode == EventKeyboard::KeyCode::KEY_3) {
    guessed = (gameSeq[currentSeqElemIdx] == 3);
  }
  else if (keyCode == EventKeyboard::KeyCode::KEY_4) {
    guessed = (gameSeq[currentSeqElemIdx] == 4);
  }

  if (!guessed) {
    processWaitingOver();
    return;
  }

  FadeOut* fout = FadeOut::create(kTimeHideWhiteRocket);
  whiteRocket->runAction(fout);

  currentSeqElemIdx++;
  C6_D4(c6, "Now  currentSeqElemIdx ", currentSeqElemIdx, " and seqElementsCount =",
        seqElementsCount );
  if (currentSeqElemIdx>= seqElementsCount) {
    doNextIteration();
    return;
  }

  //else
  launchWhiteRocket();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::processWaitingOver() {
  C6_D1(c6, "here");

  FadeOut* fout = FadeOut::create(kTimeHideWhiteRocket);
  whiteRocket->runAction(fout);

  isWaitingForKey = false;

  sceneCallback(false);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::showNextDigit() {
  currentSeqElemIdx++;
  if (currentSeqElemIdx< seqElementsCount) {
    digitNodes[gameSeq[currentSeqElemIdx]-1]->showSelf(kTimeShowOneDigit);
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::startNewGame(SceneCallback newSceneCallback) {
  if (isWaitingForKey) {
    C6_D1(c6, "No new game until current game is finished");
    return;
  }

  iterationsCount = 8;
  prepareGameSeq();
  currentIteration = 0;

  sceneCallback = newSceneCallback;
  doNextIteration();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::startWaiting() {
  C6_D1(c6, "Player turn starts");
  isWaitingForKey = true;

  currentSeqElemIdx = 0;
  launchWhiteRocket();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::unloadSpriteCache() {
  AnimationCache::getInstance()->removeAnimation(DigitNode::kFireRingAnimationName);

  SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(kPlistFileName);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
