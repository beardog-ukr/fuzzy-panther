#include "flowers/TileNode.h"
#include "flowers/ZOrderValues.h"
#include "flowers/TileType.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace flowers;

static const int kTileSize = 64;
const std::string TileNode::kFlagAnimationName = "flowers/green_flag";

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

TileNode::TileNode() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

TileNode::~TileNode() {

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

TileNode* TileNode::create(std::shared_ptr<SixCatsLogger> inc6) {
  TileNode *result = new (std::nothrow) TileNode();

  if (result != nullptr) {
    result->setLogger(inc6);
  }

  if (!result->initSelf()) {
    CC_SAFE_DELETE(result);
    return nullptr;
  }

  result->autorelease();
  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool TileNode::initMineCounter() {
  Label* label = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 32);
  label->setTextColor(Color4B(230,240,240,255));
  label->setAnchorPoint(Vec2(0.5,0.5));
  const Size cs = getContentSize();
  label->setPosition(cs.width/2, cs.height/2);
  addChild(label, kCounterZOrder);

  label->setVisible(false);

  mineCounter = label;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void TileNode::reset() {
  if (mineCounter) {
    mineCounter->setVisible(false);
  }
  if (flagNode) {
    flagNode->setVisible(false);
  }

  setTileType(kStonesTileType);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool TileNode::initFlag() {
  const string fn = "green_flag_00.png";
  flagNode = Sprite::createWithSpriteFrameName(fn);
  if (flagNode == nullptr) {
    C6_C2(c6, "Failed to init with file ", fn);
    return false;
  }

  flagNode->setPosition(Vec2(kTileSize/2, kTileSize/2));
  addChild(flagNode, kRedFlagZOrder);

  Animation* animation = AnimationCache::getInstance()->getAnimation(kFlagAnimationName);
  Animate* animate = Animate::create(animation);
  RepeatForever* ra =  RepeatForever::create(animate);
  flagNode->runAction(ra);

  flagNode->setVisible(false);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool TileNode::initSelf() {
  const string fn = "stone.png";
  if (!initWithSpriteFrameName(fn)) {
    C6_C2(c6, "Failed to init with file ", fn);
    return false;
  }

  currentTileType = kStonesTileType;
  currentMineCounterValue = 0;

  mineCounter = nullptr;
  flagNode = nullptr;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void TileNode::setMineCounter(const int value) {
  if (value ==0) {
    return;
  }

  currentMineCounterValue = value;

  if (mineCounter==nullptr) {
    if (!initMineCounter()) {
      return;
    }
  }

  ostringstream ss;
  ss << value;
  mineCounter->setString(ss.str());

  bool isStones = (currentTileType == kStonesTileType) || (currentTileType == kStonesUpTileType);
  if (!isStones) {
    mineCounter->setVisible(true);
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void TileNode::setTileType(const TileType newTileType) {
  if (currentTileType == newTileType) {
    return;
  }

  string sfn = ""; // sprite frame name
  switch (newTileType) {
  case kGrassTileType:
    sfn = "grass.png";
    break;
  case kGrassUpTileType:
    sfn = "grass_up.png";
    break;

  case kStonesTileType:
    sfn = "stone.png";
    break;
  case kStonesUpTileType:
    sfn = "stone_up.png";
    break;
  default:
    C6_C2(c6, "Unknown tile type ", (int) newTileType);
    return;
  }

  currentTileType = newTileType;
  if ((newTileType == kGrassTileType)||(newTileType==kGrassUpTileType)) {
    if (currentMineCounterValue != 0) {
//          setMineCounter(currentMineCounterValue);
      mineCounter->setVisible(true);
    }
  }

  setSpriteFrame(sfn);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void TileNode::switchFlag() {
  if (flagNode==nullptr) {
    if (!initFlag()) {
      return;
    }
  }

  flagNode->setVisible(!flagNode->isVisible());
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
