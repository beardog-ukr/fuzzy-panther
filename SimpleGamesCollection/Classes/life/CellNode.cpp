#include "life/CellNode.h"
#include "life/ZOrderValues.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>
#include <iomanip> // setfill, setw

USING_NS_CC;
using namespace std;

using namespace life;

const int CellNode::kCellSide = 16;
const float CellNode::kCellSideF = 16.0;
static const string kPlistFileName = "life/life.plist";
static const string kAliveMarkFrameName = "life/mark_alive.png";
static const string kKillMarkFrameName = "life/mark_kill.png";
static const string kCreatureAnimationName = "life/creatureAnim";

static const string kCreatureAnimationFrame1 = "life/creature/slime_idle_anim_f0.png";
static const string kCreatureAnimationFrame2 = "life/creature/slime_idle_anim_f2.png";
static const string kCreatureAnimationFrame3 = "life/creature/slime_idle_anim_f4.png";

enum life::ChangeType : int {
  kDieChange,
  kResurrectChange,
  kNothingChange
};


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CellNode::CellNode() {
  nextChange = kNothingChange;
  neighborsCount = 0;
  for (int i = 0; i<8; i++) {
    neighborNodes[i] = 0;
  }
  isAlive = false;
  nextChangeIsProtected = false;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CellNode::~CellNode() {

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void CellNode::addNeighborNode(CellNode* node) {
  if (neighborsCount >=8) {
    return;
  }

  neighborNodes[neighborsCount] = node;
  neighborsCount++;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void CellNode::applyChanges() {
  switch (nextChange) {
  case kResurrectChange:
    doResurrect();
    break;
  case kDieChange:
    doDie();
    break;
    //no default section expected here
  }

  nextChangeIsProtected = false;
  nextChange = kNothingChange;
  if (markNode != nullptr) {
    markNode->setVisible(false);
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool CellNode::checkIsAlive() const {
  return isAlive;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CellNode* CellNode::create(std::shared_ptr<SixCatsLogger> inc6) {
  CellNode *result = new (std::nothrow) CellNode();

  if (result == nullptr) {
    return result;
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

void CellNode::doDie() {
  if (creatureNode != nullptr) {
    creatureNode->setVisible(false);
  }
  isAlive = false;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void CellNode::doResurrect() {
  if (creatureNode == nullptr) {
    creatureNode = Sprite::createWithSpriteFrameName(kCreatureAnimationFrame2);
    if (creatureNode == nullptr) {
      return;
    }

    creatureNode->setPosition(Vec2(kCellSide/2, kCellSide/2));
    addChild(creatureNode, kCreatureZOrder);


    Animation* animation = AnimationCache::getInstance()->getAnimation(kCreatureAnimationName);
    Animate* animate = Animate::create(animation);
    creatureNode->runAction(RepeatForever::create(animate));
  }
  else {
    creatureNode->setVisible(true);
  }

  isAlive = true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

std::string CellNode::generateRandomFrameName() const {
  const int kMaxTileIdx = 17;
  int rv = RandomHelper::random_int((int)1, kMaxTileIdx);

  ostringstream ss;
  ss << "life/floor/floor_";
  ss << std::setfill('0') << std::setw(2) << rv;
  ss << ".png";

  return ss.str();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool CellNode::initMarkNode() {
  markNode = Sprite::createWithSpriteFrameName(kAliveMarkFrameName);
  if (markNode == nullptr) {
    C6_C2(c6, "Failed to init with file ", kAliveMarkFrameName);
    return false;
  }

  markNode->setPosition(Vec2(kCellSide/2, kCellSide/2));
  addChild(markNode, kLifeMarkZOrder);

  markNode->setVisible(false);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool CellNode::initSelf() {
  const string fn = generateRandomFrameName(); // "life/floor/floor_01.png";
  if (!initWithSpriteFrameName(fn)) {
    C6_C2(c6, "Failed to init with file ", fn);
    return false;
  }

  markNode = nullptr;
  creatureNode = nullptr;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool CellNode::loadSpriteCache(std::shared_ptr<SixCatsLogger> c6) {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();

  sfc->addSpriteFramesWithFile(kPlistFileName);
  if (!sfc->isSpriteFramesWithFileLoaded(kPlistFileName)) {
    C6_C2(c6, "Failed to find ", kPlistFileName);
    return false;
  }

  Vector<SpriteFrame*> animFrames;
  Animation *creatureAnimation = Animation::create();

  SpriteFrame *sf;
  sf = sfc->getSpriteFrameByName(kCreatureAnimationFrame1);
  if (sf == nullptr) {
    return false;
  }
  creatureAnimation->addSpriteFrame(sf);
  sf = sfc->getSpriteFrameByName(kCreatureAnimationFrame2);
  if (sf == nullptr) {
    return false;
  }
  creatureAnimation->addSpriteFrame(sf);
  sf = sfc->getSpriteFrameByName(kCreatureAnimationFrame3);
  if (sf == nullptr) {
    return false;
  }
  creatureAnimation->addSpriteFrame(sf);

  creatureAnimation->setDelayPerUnit(0.4);
  AnimationCache::getInstance()->addAnimation(creatureAnimation, kCreatureAnimationName);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void CellNode::markAlive() {
  if (markNode == nullptr) {
    if (!initMarkNode()) {
      return;
    }
  }

  markNode->setSpriteFrame(kAliveMarkFrameName);
  markNode->setVisible(true);

  nextChange = kResurrectChange;
  nextChangeIsProtected = true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void CellNode::markKill() {
  if (markNode == nullptr) {
    if (!initMarkNode()) {
      return;
    }
  }

  markNode->setSpriteFrame(kKillMarkFrameName);
  markNode->setVisible(true);

  nextChange = kDieChange;
  nextChangeIsProtected = true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void CellNode::prepareNextChange() {
  if (nextChangeIsProtected) {
    return;
  }

  nextChange = kNothingChange;

  int aliveCnt = 0;
  for (int i = 0; i<neighborsCount; i++) {
    if ((neighborNodes[i]!=nullptr)&& (neighborNodes[i]->checkIsAlive())) {
      aliveCnt++;
    }
  }

  if (isAlive) {
    if (!((aliveCnt==2)||(aliveCnt==3))) {
      nextChange = kDieChange;
    }
  }
  else {
    if (aliveCnt==3) {
      nextChange = kResurrectChange;
    }
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void CellNode::unloadSpriteCache() {
  AnimationCache::getInstance()->removeAnimation(kCreatureAnimationName);

  SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(kPlistFileName);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
